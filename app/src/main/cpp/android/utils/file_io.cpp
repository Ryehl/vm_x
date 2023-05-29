// Copyright 2016 The Android Open Source Project
//
// This software is licensed under the terms of the GNU General Public
// License version 2, as published by the Free Software Foundation, and
// may be copied, distributed, and modified under those terms.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.

#include "android/utils/file_io.h"
#include "android/base/files/Fd.h"
#include "android/base/memory/ScopedPtr.h"

#include <errno.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>

// Provide different macros for different number of string arguments where each
// string argument requires conversion
#define WIDEN_CALL_1(func, str1, ...) func(str1 , ##__VA_ARGS__)
#define WIDEN_CALL_2(func, str1, str2, ...) func(str1, str2 , ##__VA_ARGS__)

extern "C" {

FILE* android_fopen(const char* path, const char* mode) {
    return WIDEN_CALL_2(fopen, path, mode);
}

FILE* android_popen(const char* path, const char* mode) {
    return WIDEN_CALL_2(popen, path, mode);
}

int android_open_without_mode(const char* path, int flags) {
    int res = WIDEN_CALL_1(open, path, flags | O_CLOEXEC);
    android::base::fdSetCloexec(res);
    return res;
}

int android_open_with_mode(const char* path, int flags, mode_t mode) {
    int res = WIDEN_CALL_1(open, path, flags | O_CLOEXEC, mode);
    android::base::fdSetCloexec(res);
    return res;
}

int android_stat(const char* path, struct stat* buf) {
    return stat(path, buf);
}

int android_lstat(const char* path, struct stat* buf) {
    return lstat(path, buf);
}

int android_access(const char* path, int mode) {
    return WIDEN_CALL_1(access, path, mode);
}

int android_mkdir(const char* path, mode_t mode) {
    return mkdir(path, mode);
}

int android_creat(const char* path, mode_t mode) {
    return android_open(path, O_CREAT | O_WRONLY | O_TRUNC, mode);
}

int android_unlink(const char* path) {
    return WIDEN_CALL_1(unlink, path);
}

int android_chmod(const char* path, mode_t mode) {
    return WIDEN_CALL_1(chmod, path, mode);
}

}  // extern "C"

