/* Copyright (C) 2007-2009 The Android Open Source Project
**
** This software is licensed under the terms of the GNU General Public
** License version 2, as published by the Free Software Foundation, and
** may be copied, distributed, and modified under those terms.
**
** This program is distributed in the hope that it will be useful,
** but WITHOUT ANY WARRANTY; without even the implied warranty of
** MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
** GNU General Public License for more details.
*/
#include "android/base/ArraySize.h"
#include "android/base/files/PathUtils.h"
#include "android/base/memory/ScopedPtr.h"
#include "android/utils/bufprint.h"
#include "android/utils/dirscanner.h"
#include "android/utils/debug.h"
#include "android/utils/eintr_wrapper.h"
#include "android/utils/file_io.h"
#include "android/utils/path.h"

#include "android/base/files/ScopedFileHandle.h"

#include <memory>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <errno.h>
#include <fcntl.h>

#include <unistd.h>
#include <sys/stat.h>
#include <time.h>
#include <signal.h>


#define  D(...)  VERBOSE_PRINT(init,__VA_ARGS__)

using android::base::PathUtils;

/** PATH HANDLING ROUTINES
 **
 **  path_parent() can be used to return the n-level parent of a given directory
 **  this understands . and .. when encountered in the input path
 **/

static __inline__ int
ispathsep(int  c)
{
    return (c == '/');
}

char*
path_parent( const char*  path, int  levels )
{
    const char*  end = path + strlen(path);
    char*        result;

    while (levels > 0) {
        const char*  base;

        /* trim any trailing path separator */
        while (end > path && ispathsep(end[-1]))
            end--;

        base = end;
        while (base > path && !ispathsep(base[-1]))
            base--;

        if (base <= path) {
            if (end == base+1 && base[0] == '.' && levels == 1)
                return strdup("..");
          /* we can't go that far */
            return NULL;
        }

        if (end == base+1 && base[0] == '.')
            goto Next;

        if (end == base+2 && base[0] == '.' && base[1] == '.') {
            levels += 1;
            goto Next;
        }

        levels -= 1;

    Next:
        end = base - 1;
    }
    result = reinterpret_cast<char*>(malloc( end-path+1 ));
    if (result != NULL) {
        memcpy( result, path, end-path );
        result[end-path] = 0;
    }
    return result;
}

/** MISC FILE AND DIRECTORY HANDLING
 **/

/* try to make a directory. returns 0 on success, -1 on failure
 * (error code in errno) */
APosixStatus
path_mkdir( const char*  path, int  mode )
{
    return HANDLE_EINTR(android_mkdir(path, mode));
}

static APosixStatus
path_mkdir_recursive( char*  path, unsigned  len, int  mode )
{
    char      old_c;
    int       ret;
    unsigned  len2;

    /* get rid of trailing separators */
    while (len > 0 && ispathsep(path[len-1]))
        len -= 1;

    if (len == 0) {
        errno = ENOENT;
        return -1;
    }

    /* check that the parent exists, 'len2' is the length of
     * the parent part of the path */
    len2 = len-1;
    while (len2 > 0 && !ispathsep(path[len2-1]))
        len2 -= 1;

    if (len2 > 0) {
        old_c      = path[len2];
        path[len2] = 0;
        ret        = 0;
        if ( !path_exists(path) ) {
            /* the parent doesn't exist, so try to create it */
            ret = path_mkdir_recursive( path, len2, mode );
        }
        path[len2] = old_c;

        if (ret < 0)
            return ret;
    }

    /* at this point, we now the parent exists */
    old_c     = path[len];
    path[len] = 0;
    ret       = path_mkdir( path, mode );
    path[len] = old_c;

    return ret;
}

/* ensure that a given directory exists, create it if not,
   0 on success, -1 on failure (error code in errno) */
APosixStatus
path_mkdir_if_needed( const char*  path, int  mode )
{
    int  ret = 0;

    if (!path_exists(path)) {
        ret = path_mkdir(path, mode);

        if (ret < 0 && errno == ENOENT) {
            char      temp[MAX_PATH];
            unsigned  len = (unsigned)strlen(path);

            if (len > sizeof(temp)-1) {
                errno = EINVAL;
                return -1;
            }
            memcpy( temp, path, len );
            temp[len] = 0;

            return path_mkdir_recursive(temp, len, mode);
        }
    }
    return ret;
}

/* return the size of a given file in '*psize'. returns 0 on
 * success, -1 on failure (error code in errno) */
APosixStatus
path_get_size( const char*  path, uint64_t  *psize )
{
    struct stat  st;
    int ret = HANDLE_EINTR(stat(path, &st));
    if (ret == 0) {
        *psize = (uint64_t) st.st_size;
    }
    return ret;
}

/** OTHER FILE UTILITIES
 **
 **  path_empty_file() creates an empty file at a given path location.
 **  if the file already exists, it is truncated without warning
 **
 **  path_copy_file() copies one file into another.
 **
 **  both functions return 0 on success, and -1 on error
 **/

APosixStatus
path_is_same(const char* left, const char* right, bool* isSame) {
    struct stat leftStat;
    int status = stat(left, &leftStat);
    if (status != 0) {
        if (errno == ENOENT || errno == ENOTDIR) {
            // If a file doesn't exist it can't be the same as the other
            *isSame = false;
            return 0;
        }
        return status;
    }
    struct stat rightStat;
    status = stat(right, &rightStat);
    if (status != 0) {
        if (errno == ENOENT || errno == ENOTDIR) {
            // If a file doesn't exist it can't be the same as the other
            *isSame = false;
            return 0;
        }
        return status;
    }
    *isSame =
        leftStat.st_dev == rightStat.st_dev &&
        leftStat.st_ino == rightStat.st_ino;
    return 0;
}

APosixStatus
path_empty_file( const char*  path )
{
    /* on Unix, only allow the owner to read/write, since the file *
     * may contain some personal data we don't want to see exposed */
    int fd = creat(path, S_IRUSR | S_IWUSR);
    if (fd >= 0) {
        close(fd);
        return 0;
    }
    return -1;
}

template <int BufferSize>
APosixStatus path_copy_file_impl(const char*  dest, const char*  source) {
    int result = -1;
    const int fd = creat(dest, S_IRUSR | S_IWUSR);
    const int fs = open(source, S_IREAD);
    if (fs >= 0 && fd >= 0) {
        struct stat st;
        if (HANDLE_EINTR(fstat(fs, &st)) == 0) {
            posix_fadvise(fs, 0, st.st_size, POSIX_FADV_WILLNEED);
            posix_fadvise(fs, 0, st.st_size, POSIX_FADV_SEQUENTIAL);
        }
        char buf[BufferSize];
        ssize_t n;
        result = 0; /* success */
        while ((n = HANDLE_EINTR(read(fs, buf, sizeof(buf)))) != 0) {
            if (HANDLE_EINTR(write(fd, buf, n)) != n) {
                /* write failed. Make it return -1 so that an
                 * empty file be created. */
                D("Failed to copy '%s' to '%s': %s (%d)",
                       source, dest, strerror(errno), errno);
                result = -1;
                break;
            }
        }
    }

    if (fs >= 0) {
        close(fs);
    }
    if (fd >= 0) {
        close(fd);
    }
    return result;
}

APosixStatus
path_copy_file( const char*  dest, const char*  source )
{
    bool isSameFile = false;
    int status = path_is_same(source, dest, &isSameFile);
    if (status != 0 || isSameFile) {
        return status;
    }
    if (android_access(source, R_OK) < 0) {
        D("%s: source file is un-readable: %s\n",
          __FUNCTION__, source);

        // If the |source| exists but unreadable, create empty |dest| before
        // failing.
        if (android_access(source, F_OK) == 0) {
            path_empty_file(dest);
        }

        return -1;
    }

    return path_copy_file_impl<65536>(dest, source);
}

APosixStatus path_copy_file_safe(const char* dest, const char* source)
{
    bool isSameFile = false;
    int status = path_is_same(source, dest, &isSameFile);
    if (status != 0 || isSameFile) {
        return status;
    }
    return path_copy_file_impl<1024>(dest, source);
}

APosixStatus
path_delete_file( const char*  path )
{
    int ret = android_unlink(path);
    return ret;
}


void*
path_load_file(const char *fn, size_t  *pSize)
{
    char*  data;
    int    sz;
    int    fd;

    if (pSize)
        *pSize = 0;

    data   = NULL;

    fd = android_open(fn, O_BINARY | O_RDONLY);
    if(fd < 0) return NULL;

    do {
        sz = lseek(fd, 0, SEEK_END);
        if(sz < 0) break;

        if (pSize)
            *pSize = (size_t) sz;

        if (lseek(fd, 0, SEEK_SET) != 0)
            break;

        data = (char*) malloc(sz + 1);
        if(data == NULL) break;

        if (read(fd, data, sz) != sz)
            break;

        close(fd);
        data[sz] = 0;

        return data;
    } while (0);

    close(fd);

    if(data != NULL)
        free(data);

    return NULL;
}
#  define DIR_SEP  ':'

char*
path_search_exec( const char* filename )
{
    const char* sysPath = getenv("PATH");
    char        temp[PATH_MAX];
    const char* p;

    /* If the file contains a directory separator, don't search */
    if (strchr(filename, '/') != NULL) {
        if (path_exists(filename)) {
            return strdup(filename);
        } else {
            return NULL;
        }
    }

    /* If system path is empty, don't search */
    if (sysPath == NULL || sysPath[0] == '\0') {
        return NULL;
    }

    /* Count the number of non-empty items in the system path
     * Items are separated by DIR_SEP, and two successive separators
     * correspond to an empty item that will be ignored.
     * Also compute the required string storage length. */
    p = sysPath;

    while (*p) {
        const char* p2 = strchr(p, DIR_SEP);
        int   len;
        if (p2 == NULL) {
            len = strlen(p);
        } else {
            len = p2 - p;
        }

        do {
            if (len <= 0)
                break;

            snprintf(temp, sizeof(temp), "%.*s/%s", len, p, filename);

            if (path_exists(temp) && path_can_exec(temp)) {
                return strdup(temp);
            }

        } while (0);

        p += len;
        if (*p == DIR_SEP)
            p++;
    }

    /* Nothing, really */
    return NULL;
}

char *
path_escape_path(const char* src)
{
    if (!src) return NULL;

    // Allocate for the maximum output size, including terminator
    char *retStr = reinterpret_cast<char*>(malloc(2 * strlen(src) + 1));
    if (retStr == 0) return 0;

    char *dst = retStr;
    while (*src != '\0') {
        switch (*src) {
            case '=':  *dst++ = '%'; *dst++ = 'E'; break;
            case ',':  *dst++ = '%'; *dst++ = 'C'; break;
            case '%':  *dst++ = '%'; *dst++ = 'P'; break;
            default:   *dst++ = *src;
        }
        src++;
    }
    *dst = '\0';
    return retStr;
}


void
path_unescape_path(char* str)
{
    char *src = str;
    char *dst = str;

    while (*src != '\0') {
        if (*src != '%') {
            *dst++ = *src++;
        } else {
            // Escaped character
            src++;
            switch (*src) {
                case 'C': *dst++ = ','; src++; break;
                case 'E': *dst++ = '='; src++; break;
                case 'P': *dst++ = '%'; src++; break;

                default:   ;   // Just drop the '%'
            }
        }
    }
    *dst = '\0';
}

char* path_join(const char* part1, const char* part2) {
  return strdup(PathUtils::join(part1, part2).c_str());
}

APosixStatus path_copy_dir(const char* dst, const char* src) {
    bool isSameDir = false;
    int status = path_is_same(src, dst, &isSameDir);
    if (status != 0 || isSameDir) {
        return status;
    }
    auto dirScanner = android::base::makeCustomScopedPtr(
                            dirScanner_new(src), dirScanner_free);
    if (!dirScanner) return false;
    if (path_mkdir_if_needed(dst, 0777) < 0) return -1;
    const char* baseName = dirScanner_next(dirScanner.get());
    while (baseName) {
        std::string fullDstName = PathUtils::join(dst, baseName);
        std::string fullSrcName = PathUtils::join(src, baseName);
        if (path_is_dir(fullSrcName.c_str())) {
            if (path_copy_dir(fullDstName.c_str(), fullSrcName.c_str()) < 0) {
                return -1;
            }
        } else {
            if (path_copy_file(fullDstName.c_str(), fullSrcName.c_str()) < 0) {
                return -1;
            }
        }
        baseName = dirScanner_next(dirScanner.get());
    }
    return 0;
}

APosixStatus path_delete_dir(const char* path) {
    auto dirScanner = android::base::makeCustomScopedPtr(dirScanner_new(path),
                                                         dirScanner_free);
    if (!dirScanner)
        return -EINVAL;

    int fullRes = 0;
    const char* name = dirScanner_nextFull(dirScanner.get());
    while (name) {
        if (path_is_dir(name)) {
            if (auto res = path_delete_dir(name)) {
                fullRes = fullRes ? fullRes : res;
            }
        } else {
            if (auto res = path_delete_file(name)) {
                fullRes = fullRes ? fullRes : res;
            }
        }
        name = dirScanner_nextFull(dirScanner.get());
    }

    auto res = rmdir(path);
    fullRes = fullRes ? fullRes : res;
    return fullRes;
}
