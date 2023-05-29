// Copyright (C) 2015 The Android Open Source Project
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
// http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

#include "android/base/system/System.h"

#include "android/base/EintrWrapper.h"
#include "android/base/StringFormat.h"
#include "android/base/StringParse.h"
#include "android/base/files/PathUtils.h"
#include "android/base/files/ScopedFd.h"
#include "android/base/memory/LazyInstance.h"
#include "android/base/memory/ScopedPtr.h"
#include "android/base/misc/FileUtils.h"
#include "android/base/misc/StringUtils.h"
#include "android/base/threads/Thread.h"
#include "android/utils/tempfile.h"
#include "android/utils/path.h"



#include <algorithm>
#include <array>
#include <chrono>
#include <memory>
#include <vector>
#include <unordered_set>

#ifndef _WIN32
#include <fcntl.h>
#include <dirent.h>
#include <pwd.h>
#include <signal.h>
#include <sys/statvfs.h>
#include <sys/times.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <time.h>
#include <unistd.h>
#endif
#include <assert.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/time.h>

#if defined (__linux__)
#include <fstream>
#include <string>
#endif

// This variable is a pointer to a zero-terminated array of all environment
// variables in the current process.
// Posix requires this to be declared as extern at the point of use
// NOTE: Apple developer manual states that this variable isn't available for
// the shared libraries, and one has to use the _NSGetEnviron() function instead
extern "C" char** environ;

namespace android {
namespace base {

using std::string;
using std::unique_ptr;
using std::vector;

namespace {

struct TickCountImpl {
private:
    System::WallDuration mStartTimeUs;

public:
    TickCountImpl() {
        mStartTimeUs = getUs();
    }


    System::WallDuration getStartTimeUs() const {
        return mStartTimeUs;
    }

    System::WallDuration getUs() const {
    timespec ts;
    clock_gettime(CLOCK_MONOTONIC, &ts);
    return ts.tv_sec * 1000000ll + ts.tv_nsec / 1000;
    }
};

// This is, maybe, the only static variable that may not be a LazyInstance:
// it holds the actual timestamp at startup, and has to be initialized as
// soon as possible after the application launch.
static const TickCountImpl kTickCount;

}  // namespace

namespace {

class HostSystem : public System {
public:
    HostSystem() : mProgramDir(), mHomeDir(), mAppDataDir() {}

    virtual ~HostSystem() {}

    const std::string& getProgramDirectory() const override {
        if (mProgramDir.empty()) {
            char path[1024];
            memset(path, 0, sizeof(path));  // happy valgrind!
            int len = readlink("/proc/self/exe", path, sizeof(path));
            if (len > 0 && len < (int)sizeof(path)) {
                char* x = ::strrchr(path, '/');
                if (x) {
                    *x = '\0';
                    mProgramDir.assign(path);
                }
            }
        }
        return mProgramDir;
    }

    std::string getCurrentDirectory() const override {
        char currentDir[PATH_MAX];
        if (!getcwd(currentDir, sizeof(currentDir))) {
            return std::string();
        }
        return std::string(currentDir);
    }

    const std::string& getLauncherDirectory() const override {
        if (mLauncherDir.empty()) {
            std::string launcherDirEnv = envGet("ANDROID_EMULATOR_LAUNCHER_DIR");
            if (!launcherDirEnv.empty()) {
                mLauncherDir = std::move(launcherDirEnv);
                return mLauncherDir;
            }

            const std::string& programDir = getProgramDirectory();
            std::string launcherName = PathUtils::toExecutableName("emulator");

            // Let's first check if this is qemu2 binary, which lives in
            // <launcher-dir>/qemu/<os>-<arch>/
            // look for the launcher in grandparent directory
            auto programDirVector = PathUtils::decompose(programDir);
            if (programDirVector.size() >= 2) {
                programDirVector.resize(programDirVector.size() - 2);
                std::string grandparentDir = PathUtils::recompose(programDirVector);
                programDirVector.push_back(launcherName);
                std::string launcherPath = PathUtils::recompose(programDirVector);
                if (pathIsFile(launcherPath)) {
                    mLauncherDir = std::move(grandparentDir);
                    return mLauncherDir;
                }
            }

            std::vector<StringView> pathList = {programDir, launcherName};
            std::string launcherPath = PathUtils::recompose(pathList);
            if (pathIsFile(launcherPath)) {
                mLauncherDir = programDir;
                return mLauncherDir;
            }

            mLauncherDir.assign("<unknown-launcher-dir>");
        }
        return mLauncherDir;
    }

    const std::string& getHomeDirectory() const override {
        if (mHomeDir.empty()) {
            // Try getting HOME from env first
            const char* home = getenv("HOME");
            if (home != NULL) {
                mHomeDir.assign(home);
            } else {
                // If env HOME appears empty for some reason,
                // try getting HOME by querying system password database
                const struct passwd *pw = getpwuid(getuid());
                if (pw != NULL && pw->pw_dir != NULL) {
                    mHomeDir.assign(pw->pw_dir);
                }
            }
        }
        return mHomeDir;
    }

    const std::string& getAppDataDirectory() const override {
        return mAppDataDir;
    }


    int getHostBitness() const override {


        // !__x86_64__ && !_WIN32
        // This function returns 64 if host is running 64-bit OS, or 32 otherwise.
        //
        // It uses the same technique in ndk/build/core/ndk-common.sh.
        // Here are comments from there:
        //
        // ## On Linux or Darwin, a 64-bit kernel (*) doesn't mean that the
        // ## user-land is always 32-bit, so use "file" to determine the bitness
        // ## of the shell that invoked us. The -L option is used to de-reference
        // ## symlinks.
        // ##
        // ## Note that on Darwin, a single executable can contain both x86 and
        // ## x86_64 machine code, so just look for x86_64 (darwin) or x86-64
        // ## (Linux) in the output.
        //
        // (*) ie. The following code doesn't always work:
        //     struct utsname u;
        //     int host_runs_64bit_OS = (uname(&u) == 0 &&
        //                              strcmp(u.machine, "x86_64") == 0);
        //
        // Note: system() call on MacOS disables SIGINT signal and fails
        //  to restore it back. As of now we don't have 32-bit Darwin binaries
        //  so this code path won't ever happen, but you've been warned.
        //
        if (system("file -L \"$SHELL\" | grep -q \"x86[_-]64\"") == 0) {
                return 64;
        } else if (system("file -L \"$SHELL\" > /dev/null")) {
            fprintf(stderr, "WARNING: Cannot decide host bitness because "
                    "$SHELL is not properly defined; 32 bits assumed.\n");
        }
        return 32;
    }

    OsType getOsType() const override {
        return OsType::Linux;
    }

    string getOsName() {
      static string lastSuccessfulValue;
      if (!lastSuccessfulValue.empty()) {
        return lastSuccessfulValue;
      }
        using android::base::ScopedFd;
        using android::base::trim;
        const auto versionNumFile = android::base::makeCustomScopedPtr(
              tempfile_create(), tempfile_close);

        if (!versionNumFile) {
          string errorStr =
            "Error: Internal error: could not create a temporary file";
          LOG(VERBOSE) << errorStr;
          return errorStr;
        }

        string tempPath = tempfile_path(versionNumFile.get());

        int exitCode = -1;
        vector<string> command{"lsb_release", "-d"};
        runCommand(command,
                   RunOptions::WaitForCompletion |
                           RunOptions::TerminateOnTimeout |
                           RunOptions::DumpOutputToFile,
                   1000,  // timeout ms
                   &exitCode, nullptr, tempPath);

        if (exitCode) {
          string errorStr = "Could not get host OS product version.";
          LOG(VERBOSE) << errorStr;
          return errorStr;
        }

        ScopedFd tempfileFd(open(tempPath.c_str(), O_RDONLY));
        if (!tempfileFd.valid()) {
            LOG(VERBOSE) << "Could not open" << tempPath << " : "
                         << strerror(errno);
            return "";
        }

        string contents;
        android::readFileIntoString(tempfileFd.get(), &contents);
        if (contents.empty()) {
          string errorStr = StringFormat(
              "Error: Internal error: could not read temporary file '%s'",
              tempPath);
          LOG(VERBOSE) << errorStr;
          return errorStr;
        }
        //"lsb_release -d" output is "Description:      [os-product-version]"
        lastSuccessfulValue = trim(contents.substr(12, contents.size() - 12));
        return lastSuccessfulValue;
    }

    bool isRunningUnderWine() const override {
        return false;
    }

    System::Pid getCurrentProcessId() const override
    {
        return getpid();
    }

    WaitExitResult waitForProcessExit(int pid, Duration timeoutMs) const override {
        uint64_t remainingMs = timeoutMs;
        const uint64_t pollMs = 100;

        int ret = HANDLE_EINTR(kill(pid, 0));
        if (ret < 0 && errno == ESRCH) {
            return WaitExitResult::Exited; // successfully waited out the pid
        }

        while (true) {
            sleepMs(pollMs);
            ret = HANDLE_EINTR(kill(pid, 0));
            if (ret < 0 && errno == ESRCH) {
                return WaitExitResult::Exited; // successfully waited out the pid
            }
            if (remainingMs < pollMs) {
                return WaitExitResult::Timeout; // timed out
            }
            remainingMs -= pollMs;
        }
    }


    int getCpuCoreCount() const override {
        auto res = (int)::sysconf(_SC_NPROCESSORS_ONLN);
        return res < 1 ? 1 : res;
    }

    MemUsage getMemUsage() const override {
        MemUsage res = {};
        size_t size = 0;
        std::ifstream fin;

        fin.open("/proc/self/status");
        if (!fin.good()) {
            return res;
        }

        std::string line;
        while (std::getline(fin, line)) {
            if (sscanf(line.c_str(), "VmRSS:%lu", &size) == 1) {
                res.resident = size * 1024;
            }
            else if (sscanf(line.c_str(), "VmHWM:%lu", &size) == 1) {
                res.resident_max = size * 1024;
            }
            else if (sscanf(line.c_str(), "VmSize:%lu", &size) == 1) {
                res.virt = size * 1024;
            }
            else if (sscanf(line.c_str(), "VmPeak:%lu", &size) == 1) {
                res.virt_max = size * 1024;
            }
        }
        fin.close();

        fin.open("/proc/meminfo");
        if (!fin.good()) {
            return res;
        }

        while (std::getline(fin, line)) {
            if (sscanf(line.c_str(), "MemTotal:%lu", &size) == 1) {
                res.total_phys_memory = size * 1024;
            }
            else if (sscanf(line.c_str(), "MemAvailable:%lu", &size) == 1) {
                res.avail_phys_memory = size * 1024;
            }
            else if (sscanf(line.c_str(), "SwapTotal:%lu", &size) == 1) {
                res.total_page_file = size * 1024;
            }
        }
        fin.close();

        return res;
    }

    Optional<DiskKind> pathDiskKind(StringView path) override {
        return diskKindInternal(path);
    }
    Optional<DiskKind> diskKind(int fd) override {
        return diskKindInternal(fd);
    }

    std::vector<std::string> scanDirEntries(
            StringView dirPath,
            bool fullPath = false) const override {
        std::vector<std::string> result = scanDirInternal(dirPath);
        if (fullPath) {
            // Prepend |dirPath| to each entry.
            std::string prefix = PathUtils::addTrailingDirSeparator(dirPath);
            for (auto& entry : result) {
                entry.insert(0, prefix);
            }
        }
        return result;
    }

    std::string envGet(StringView varname) const override {
        const char* value = getenv(varname.c_str());
        if (!value) {
            value = "";
        }
        return std::string(value);
    }

    void envSet(StringView varname, StringView varvalue) override {
        if (varvalue.empty()) {
            unsetenv(varname.c_str());
        } else {
            setenv(varname.c_str(), varvalue.c_str(), 1);
        }
    }

    bool envTest(StringView varname) const override {
        const char* value = getenv(varname.c_str());
        return value && value[0] != '\0';
    }

    std::vector<std::string> envGetAll() const override {
        std::vector<std::string> res;
        for (auto env = environ; env && *env; ++env) {
            res.push_back(*env);
        }
        return res;
    }

    bool isRemoteSession(std::string* sessionType) const final {
        if (envTest("NX_TEMP")) {
            if (sessionType) {
                *sessionType = "NX";
            }
            return true;
        }
        if (envTest("CHROME_REMOTE_DESKTOP_SESSION")) {
            if (sessionType) {
                *sessionType = "Chrome Remote Desktop";
            }
            return true;
        }
        if (!envGet("SSH_CONNECTION").empty() && !envGet("SSH_CLIENT").empty()) {
            // This can be a remote X11 session, let's check if DISPLAY is set
            // to something uncommon.
            if (envGet("DISPLAY").size() > 2) {
                if (sessionType) {
                    *sessionType = "X11 Forwarding";
                }
                return true;
            }
        }
        return false;
    }

    bool pathExists(StringView path) const override {
        return pathExistsInternal(path);
    }

    bool pathIsFile(StringView path) const override {
        return pathIsFileInternal(path);
    }

    bool pathIsDir(StringView path) const override {
        return pathIsDirInternal(path);
    }

    bool pathIsLink(StringView path) const override {
        return pathIsLinkInternal(path);
    }

    bool pathCanRead(StringView path) const override {
        return pathCanReadInternal(path);
    }

    bool pathCanWrite(StringView path) const override {
        return pathCanWriteInternal(path);
    }

    bool pathCanExec(StringView path) const override {
        return pathCanExecInternal(path);
    }

    bool deleteFile(StringView path) const override {
        return deleteFileInternal(path);
    }

    bool pathFileSize(StringView path,
            FileSize* outFileSize) const override {
        return pathFileSizeInternal(path, outFileSize);
    }

    FileSize recursiveSize(StringView path) const override {
        return recursiveSizeInternal(path);
    }

    bool pathFreeSpace(StringView path, FileSize* spaceInBytes) const override {
        return pathFreeSpaceInternal(path, spaceInBytes);
    }

    bool fileSize(int fd, FileSize* outFileSize) const override {
        return fileSizeInternal(fd, outFileSize);
    }

    Optional<std::string> which(StringView command) const override {
      if (PathUtils::isAbsolute(command)) {
        if (!pathCanExec(command))
          return {};

        return Optional<std::string>(command);
      }

      ScopedCPtr<char> exe(::path_search_exec(command.c_str()));
      if (exe && pathCanExec(exe.get())) {
        return Optional<std::string>(exe.get());
      }
      return {};
    }

    Optional<Duration> pathCreationTime(StringView path) const override {
        return pathCreationTimeInternal(path);
    }

    Optional<Duration> pathModificationTime(StringView path) const override {
        return pathModificationTimeInternal(path);
    }

    Times getProcessTimes() const override {
        Times res = {};

        tms times = {};
        ::times(&times);
        // convert to milliseconds
        const long int ticksPerSec = ::sysconf(_SC_CLK_TCK);
        res.systemMs = (times.tms_stime * 1000ll) / ticksPerSec;
        res.userMs = (times.tms_utime * 1000ll) / ticksPerSec;
        res.wallClockMs =
            (kTickCount.getUs() - kTickCount.getStartTimeUs()) / 1000;

        return res;
    }

    time_t getUnixTime() const override {
        return time(NULL);
    }

    Duration getUnixTimeUs() const override {
        timeval tv;
        gettimeofday(&tv, nullptr);
        return tv.tv_sec * 1000000LL + tv.tv_usec;
    }

    WallDuration getHighResTimeUs() const override {
        return kTickCount.getUs();
    }

    void sleepMs(unsigned n) const override {
        Thread::sleepMs(n);
    }

    void sleepUs(unsigned n) const override {
        Thread::sleepUs(n);
    }

    void yield() const override {
        Thread::yield();
    }

    Optional<std::string> runCommandWithResult(
            const std::vector<std::string>& commandLine,
            System::Duration timeoutMs = kInfinite,
            System::ProcessExitCode* outExitCode = nullptr) override {
        std::string tmp_dir = getTempDir();

        // Get temporary file path
        constexpr base::StringView temp_filename_pattern = "runCommand_XXXXXX";
        std::string temp_file_path =
                PathUtils::join(tmp_dir, temp_filename_pattern);

        auto tmpfd =
                android::base::ScopedFd(mkstemp((char*)temp_file_path.data()));
        if (!tmpfd.valid()) {
            return {};
        }
        //
        // We have to close the handle. On windows we will not be able to write
        // to this file, resulting in strange behavior.
        tmpfd.close();
        temp_file_path.resize(strlen(temp_file_path.c_str()));
        auto tmpFileDeleter = base::makeCustomScopedPtr(
               /* std::string* */ &temp_file_path,
               /* T? */ [](const std::string* name) { remove(name->c_str()); });

        if (!runCommand(commandLine,
                        RunOptions::WaitForCompletion |
                                RunOptions::TerminateOnTimeout |
                                RunOptions::DumpOutputToFile,
                        timeoutMs, outExitCode, nullptr, temp_file_path)) {
            return {};
        }

        // Extract stderr/stdout
        return android::readFileIntoString(temp_file_path).valueOr({});
    }

    bool runCommand(const std::vector<std::string>& commandLine,
            RunOptions options,
            System::Duration timeoutMs,
            System::ProcessExitCode* outExitCode,
            System::Pid* outChildPid,
            const std::string& outputFile) override {
        // Sanity check.
        if (commandLine.empty()) {
            return false;
        }

        sigset_t oldset;
        sigset_t set;
        if (sigemptyset(&set) || sigaddset(&set, SIGCHLD) ||
                pthread_sigmask(SIG_UNBLOCK, &set, &oldset)) {
            return false;
        }
        auto result = runCommandPosix(commandLine, options, timeoutMs,
                outExitCode, outChildPid, outputFile);
        pthread_sigmask(SIG_SETMASK, &oldset, nullptr);
        return result;
    }

    std::string getTempDir() const override {
        std::string result;
        const char* tmppath = getenv("ANDROID_TMP");
        if (!tmppath) {
            const char* user = getenv("USER");
            if (user == NULL || user[0] == '\0') {
                user = "unknown";
            }
            result = "/tmp/android-";
            result += user;
        } else {
            result = tmppath;
        }
        ::mkdir(result.c_str(), 0744);
        return result;
    }

#ifndef _WIN32
    bool runCommandPosix(const std::vector<std::string>& commandLine,
            RunOptions options,
            System::Duration timeoutMs,
            System::ProcessExitCode* outExitCode,
            System::Pid* outChildPid,
            const std::string& outputFile) {
        vector<char*> params;
        for (const auto& item : commandLine) {
            params.push_back(const_cast<char*>(item.c_str()));
        }
        params.push_back(nullptr);

        std::string cmd = "";
        if(LOG_IS_ON(VERBOSE)) {
            cmd = "|";
            for (const auto& param : commandLine) {
                cmd += param;
                cmd += " ";
            }
            cmd += "|";
        }

#if defined(__APPLE__)
        int pid = runViaPosixSpawn(commandLine[0].c_str(), params, options,
                outputFile);
#else
        int pid = runViaForkAndExec(commandLine[0].c_str(), params, options,
                outputFile);
#endif  // !defined(__APPLE__)

        if (pid < 0) {
            LOG(VERBOSE) << "Failed to fork for command " << cmd;
            return false;
        }

        if (outChildPid) {
            *outChildPid = pid;
        }

        if ((options & RunOptions::WaitForCompletion) == 0) {
            return true;
        }

        // We were requested to wait for the process to complete.
        int exitCode;
        // Do not use SIGCHLD here because we're not sure if we're
        // running on the main thread and/or what our sigmask is.
        if (timeoutMs == kInfinite) {
            // Let's just wait forever and hope that the child process
            // exits.
            HANDLE_EINTR(waitpid(pid, &exitCode, 0));
            if (outExitCode) {
                *outExitCode = WEXITSTATUS(exitCode);
            }
            return WIFEXITED(exitCode);
        }

        auto startTime = std::chrono::steady_clock::now();
        auto elapsed = std::chrono::milliseconds::zero();
        while (elapsed.count() < timeoutMs) {
            pid_t waitPid = HANDLE_EINTR(waitpid(pid, &exitCode, WNOHANG));
            if (waitPid < 0) {
                auto local_errno = errno;
                LOG(VERBOSE) << "Error running command " << cmd
                    << ". waitpid failed with |"
                    << strerror(local_errno) << "|";
                return false;
            }

            if (waitPid > 0) {
                if (outExitCode) {
                    *outExitCode = WEXITSTATUS(exitCode);
                }
                return WIFEXITED(exitCode);
            }

            sleepMs(10);
            elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(
                    std::chrono::steady_clock::now() - startTime);
        }

        // Timeout occured.
        if ((options & RunOptions::TerminateOnTimeout) != 0) {
            kill(pid, SIGKILL);
            waitpid(pid, nullptr, WNOHANG);
        }
        LOG(VERBOSE) << "Timed out with running command " << cmd;
        return false;
    }

    int runViaForkAndExec(const char* command,
            const vector<char*>& params,
            RunOptions options,
            const string& outputFile) {
        // If an output file was requested, open it before forking, since
        // creating a file in the child of a multi-threaded process is sketchy.
        //
        // It will be immediately closed in the parent process, and dup2'd into
        // stdout and stderr in the child process.
        int outputFd = 0;
        if ((options & RunOptions::DumpOutputToFile) != RunOptions::Empty) {
            if (outputFile.empty()) {
                LOG(VERBOSE) << "Can not redirect output to empty file!";
                return -1;
            }

            // Ensure the umask doesn't get in the way while creating the
            // output file.
            mode_t old = umask(0);
            outputFd = open(outputFile.c_str(), O_WRONLY | O_CREAT | O_TRUNC,
                    0700);
            umask(old);
            if (outputFd < 0) {
                LOG(VERBOSE) << "Failed to open file to redirect stdout/stderr";
                return -1;
            }
        }

        int pid = fork();

        if (pid != 0) {
            if (outputFd > 0) {
                close(outputFd);
            }
            // Return the child's pid / error code to parent process.
            return pid;
        }

        // In the child process.
        // Do not do __anything__ except execve. That includes printing to
        // stdout/stderr. None of it is safe in the child process forked from a
        // parent with multiple threads.
        if ((options & RunOptions::DumpOutputToFile) != RunOptions::Empty) {
            dup2(outputFd, 1);
            dup2(outputFd, 2);
            close(outputFd);
        } else if ((options & RunOptions::ShowOutput) == RunOptions::Empty) {
            // We were requested to RunOptions::HideAllOutput
            int fd = open("/dev/null", O_WRONLY);
            if (fd > 0) {
                dup2(fd, 1);
                dup2(fd, 2);
                close(fd);
            }
        }

        // We never want to forward our stdin to the child process. On the other
        // hand, closing it can confuse some programs.
        int fd = open("/dev/null", O_RDONLY);
        if (fd > 0) {
            dup2(fd, 0);
            close(fd);
        }

        if (execvp(command, params.data()) == -1) {
            // emulator doesn't really like exit calls from a forked process
            // (it just hangs), so let's just kill it
            if (raise(SIGKILL) != 0) {
                exit(RunFailed);
            }
        }
        // Should not happen, but let's keep the compiler happy
        return -1;
    }

#if defined(__APPLE__)
    int runViaPosixSpawn(const char* command,
            const vector<char*>& params,
            RunOptions options,
            const string& outputFile) {
        posix_spawnattr_t attr;
        if (posix_spawnattr_init(&attr)) {
            LOG(VERBOSE) << "Failed to initialize spawnattr obj.";
            return -1;
        }
        // Automatically destroy the successfully initialized attr.
        auto attrDeleter = [](posix_spawnattr_t* t) {
            posix_spawnattr_destroy(t);
        };
        unique_ptr<posix_spawnattr_t, decltype(attrDeleter)> scopedAttr(
                &attr, attrDeleter);

        if (posix_spawnattr_setflags(&attr, POSIX_SPAWN_CLOEXEC_DEFAULT)) {
            LOG(VERBOSE) << "Failed to request CLOEXEC.";
            return -1;
        }

        posix_spawn_file_actions_t fileActions;
        if (posix_spawn_file_actions_init(&fileActions)) {
            LOG(VERBOSE) << "Failed to initialize fileactions obj.";
            return -1;
        }
        // Automatically destroy the successfully initialized fileActions.
        auto fileActionsDeleter = [](posix_spawn_file_actions_t* t) {
            posix_spawn_file_actions_destroy(t);
        };
        unique_ptr<posix_spawn_file_actions_t, decltype(fileActionsDeleter)>
            scopedFileActions(&fileActions, fileActionsDeleter);

        if ((options & RunOptions::DumpOutputToFile) != RunOptions::Empty) {
            if (posix_spawn_file_actions_addopen(
                        &fileActions, 1, outputFile.c_str(),
                        O_WRONLY | O_CREAT | O_TRUNC, 0700) ||
                    posix_spawn_file_actions_addopen(
                        &fileActions, 2, outputFile.c_str(),
                        O_WRONLY | O_CREAT | O_TRUNC, 0700)) {
                LOG(VERBOSE) << "Failed to redirect child output to file "
                    << outputFile;
                return -1;
            }
        } else if ((options & RunOptions::ShowOutput) != RunOptions::Empty) {
            if (posix_spawn_file_actions_addinherit_np(&fileActions, 1) ||
                    posix_spawn_file_actions_addinherit_np(&fileActions, 2)) {
                LOG(VERBOSE) << "Failed to request child stdout/stderr to be "
                    "left intact";
                return -1;
            }
        } else {
            if (posix_spawn_file_actions_addopen(&fileActions, 1, "/dev/null",
                        O_WRONLY, 0700) ||
                    posix_spawn_file_actions_addopen(&fileActions, 2, "/dev/null",
                        O_WRONLY, 0700)) {
                LOG(VERBOSE) << "Failed to redirect child output to /dev/null";
                return -1;
            }
        }

        // We never want to forward our stdin to the child process. On the other
        // hand, closing it can confuse some programs.
        if (posix_spawn_file_actions_addopen(&fileActions, 0, "/dev/null",
                    O_RDONLY, 0700)) {
            LOG(VERBOSE) << "Failed to redirect child stdin from /dev/null";
            return -1;
        }

        // Posix spawn requires that argv[0] exists.
        assert(params[0] != nullptr);

        int pid;
        if (int error_code = posix_spawnp(&pid, command, &fileActions, &attr,
                    params.data(), environ)) {
            LOG(VERBOSE) << "posix_spawnp failed: " << strerror(error_code);
            return -1;
        }
        return pid;
    }
#endif  // defined(__APPLE__)
#endif  // !_WIN32

private:
    mutable std::string mProgramDir;
    mutable std::string mLauncherDir;
    mutable std::string mHomeDir;
    mutable std::string mAppDataDir;
};

LazyInstance<HostSystem> sHostSystem = LAZY_INSTANCE_INIT;
System* sSystemForTesting = NULL;

#ifdef _WIN32
// Return |path| as a Unicode string, while discarding trailing separators.
Win32UnicodeString win32Path(StringView path) {
    Win32UnicodeString wpath(path);
    // Get rid of trailing directory separators, Windows doesn't like them.
    size_t size = wpath.size();
    while (size > 0U &&
           (wpath[size - 1U] == L'\\' || wpath[size - 1U] == L'/')) {
        size--;
    }
    if (size < wpath.size()) {
        wpath.resize(size);
    }
    return wpath;
}

using PathStat = struct _stat64;

#else  // _WIN32

using PathStat = struct stat;

#endif  // _WIN32

int pathStat(StringView path, PathStat* st) {
#ifdef _WIN32
    return _wstat64(win32Path(path).c_str(), st);
#else   // !_WIN32
    return HANDLE_EINTR(stat(path.c_str(), st));
#endif  // !_WIN32
}

int fdStat(int fd, PathStat* st) {
#ifdef _WIN32
    return fstat64(fd, st);
#else   // !_WIN32
    return HANDLE_EINTR(fstat(fd, st));
#endif  // !_WIN32
}

int pathAccess(StringView path, int mode) {
#ifdef _WIN32
    // Convert |mode| to win32 permission bits.
    int win32mode = 0x0;
    if ((mode & R_OK) || (mode & X_OK)) {
        win32mode |= 0x4;
    }
    if (mode & W_OK) {
        win32mode |= 0x2;
    }
    return _waccess(win32Path(path).c_str(), win32mode);
#else   // !_WIN32
    return HANDLE_EINTR(access(path.c_str(), mode));
#endif  // !_WIN32
}

}  // namespace

// static
System* System::get() {
    System* result = sSystemForTesting;
    if (!result) {
        result = hostSystem();
    }
    return result;
}

#ifdef __x86_64__
// static
const char* System::kLibSubDir = "lib64";
// static
const char* System::kBinSubDir = "bin64";
#else
// static
const char* System::kLibSubDir = "lib";
// static
const char* System::kBinSubDir = "bin";
#endif

const char* System::kBin32SubDir = "bin";

// These need to be defined so one can take an address of them.
const int System::kProgramBitness;
const char System::kDirSeparator;
const char System::kPathSeparator;

#ifdef _WIN32
// static
const char* System::kLibrarySearchListEnvVarName = "PATH";
#elif defined(__APPLE__)
const char* System::kLibrarySearchListEnvVarName = "DYLD_LIBRARY_PATH";
#else
// static
const char* System::kLibrarySearchListEnvVarName = "LD_LIBRARY_PATH";
#endif

// static
System* System::setForTesting(System* system) {
    System* result = sSystemForTesting;
    sSystemForTesting = system;
    return result;
}

System* System::hostSystem() {
    return sHostSystem.ptr();
}

// static
std::vector<std::string> System::scanDirInternal(StringView dirPath) {
    std::vector<std::string> result;

    if (dirPath.empty()) {
        return result;
    }
#ifdef _WIN32
    auto root = PathUtils::addTrailingDirSeparator(dirPath);
    root += '*';
    Win32UnicodeString rootUnicode(root);
    struct _wfinddata_t findData;
    intptr_t findIndex = _wfindfirst(rootUnicode.c_str(), &findData);
    if (findIndex >= 0) {
        do {
            const wchar_t* name = findData.name;
            if (wcscmp(name, L".") != 0 && wcscmp(name, L"..") != 0) {
                result.push_back(Win32UnicodeString::convertToUtf8(name));
            }
        } while (_wfindnext(findIndex, &findData) >= 0);
        _findclose(findIndex);
    }
#else  // !_WIN32
    DIR* dir = ::opendir(dirPath.c_str());
    if (dir) {
        for (;;) {
            struct dirent* entry = ::readdir(dir);
            if (!entry) {
                break;
            }
            const char* name = entry->d_name;
            if (strcmp(name, ".") != 0 && strcmp(name, "..") != 0) {
                result.push_back(std::string(name));
            }
        }
        ::closedir(dir);
    }
#endif  // !_WIN32
    std::sort(result.begin(), result.end());
    return result;
}

// static
bool System::pathIsLinkInternal(StringView path) {
#ifdef _WIN32
    // Supposedly GetFileAttributes() and FindFirstFile()
    // can be used to detect symbolic links. In my tests,
    // a symbolic link looked exactly like a regular file.
    return false;
#else
    struct stat fileStatus;
    if (lstat(path.c_str(), &fileStatus)) {
        return false;
    }
    return S_ISLNK(fileStatus.st_mode);
#endif
}

// static
bool System::pathExistsInternal(StringView path) {
    if (path.empty()) {
        return false;
    }
    int ret = pathAccess(path, F_OK);
    return (ret == 0) || (errno != ENOENT);
}

// static
bool System::pathIsFileInternal(StringView path) {
    if (path.empty()) {
        return false;
    }
    PathStat st;
    int ret = pathStat(path, &st);
    if (ret < 0) {
        return false;
    }
    return S_ISREG(st.st_mode);
}

// static
bool System::pathIsDirInternal(StringView path) {
    if (path.empty()) {
        return false;
    }
    PathStat st;
    int ret = pathStat(path, &st);
    if (ret < 0) {
        return false;
    }
    return S_ISDIR(st.st_mode);
}

// static
bool System::pathCanReadInternal(StringView path) {
    if (path.empty()) {
        return false;
    }
    return pathAccess(path, R_OK) == 0;
}

// static
bool System::pathCanWriteInternal(StringView path) {
    if (path.empty()) {
        return false;
    }
    return pathAccess(path, W_OK) == 0;
}

// static
bool System::pathCanExecInternal(StringView path) {
    if (path.empty()) {
        return false;
    }
    return pathAccess(path, X_OK) == 0;
}

bool System::deleteFileInternal(StringView path) {
    if (!pathIsFileInternal(path)) {
        return false;
    }

    int remove_res = -1;

#ifdef _WIN32
    remove_res = remove(path.c_str());
    if (remove_res < 0) {
        // Windows sometimes just fails to delete a file
        // on the first try.
        // Sleep a little bit and try again here.
        System::get()->sleepMs(1);
        remove_res = remove(path.c_str());
    }
#else
    remove_res = remove(path.c_str());
#endif

    if (remove_res != 0) {
        LOG(VERBOSE) << "Failed to delete file [" << path << "].";
    }

    return remove_res == 0;
}

bool System::pathFreeSpaceInternal(StringView path, FileSize* spaceInBytes) {
#ifdef _WIN32
    ULARGE_INTEGER freeBytesAvailableToUser;
    bool result = GetDiskFreeSpaceEx(path.c_str(), &freeBytesAvailableToUser, NULL, NULL);
    if (!result) {
        return false;
    }
    *spaceInBytes = freeBytesAvailableToUser.QuadPart;
    return true;
#else
    struct statvfs fsStatus;
    int result = statvfs(path.c_str(), &fsStatus);
    if (result != 0) {
        return false;
    }
    // Available space is (block size) * (# free blocks)
    *spaceInBytes = ((FileSize)fsStatus.f_frsize) * fsStatus.f_bavail;
    return true;
#endif
}

// static
bool System::pathFileSizeInternal(StringView path, FileSize* outFileSize) {
    if (path.empty() || !outFileSize) {
        return false;
    }
    PathStat st;
    int ret = pathStat(path, &st);
    if (ret < 0 || !S_ISREG(st.st_mode)) {
        return false;
    }
    // This is off_t on POSIX and a 32/64 bit integral type on windows based on
    // the host / compiler combination. We cast everything to 64 bit unsigned to
    // play safe.
    *outFileSize = static_cast<FileSize>(st.st_size);
    return true;
}

// static
System::FileSize System::recursiveSizeInternal(StringView path) {

    std::vector<std::string> fileList;
    fileList.push_back(path);

    FileSize totalSize = 0;

    while (fileList.size() > 0) {
        const auto currentPath = std::move(fileList.back());
        fileList.pop_back();
        if (pathIsFileInternal(currentPath) || pathIsLinkInternal(currentPath)) {
            // Regular file or link. Return its size.
            FileSize theSize;
            if (pathFileSizeInternal(currentPath, &theSize)) {
                totalSize += theSize;
            }
        } else if (pathIsDirInternal(currentPath)) {
            // Directory. Add its contents to the list.
            std::vector<std::string> includedFiles = scanDirInternal(currentPath);
            for (const auto& file : includedFiles) {
                fileList.push_back(PathUtils::join(currentPath, file));
            }
        }
    }
    return totalSize;
}

bool System::fileSizeInternal(int fd, System::FileSize* outFileSize) {
    if (fd < 0) {
        return false;
    }
    PathStat st;
    int ret = fdStat(fd, &st);
    if (ret < 0 || !S_ISREG(st.st_mode)) {
        return false;
    }
    // This is off_t on POSIX and a 32/64 bit integral type on windows based on
    // the host / compiler combination. We cast everything to 64 bit unsigned to
    // play safe.
    *outFileSize = static_cast<FileSize>(st.st_size);
    return true;
}

// static
Optional<System::Duration> System::pathCreationTimeInternal(StringView path) {
    // on Linux.
    return {};
}

// static
Optional<System::Duration> System::pathModificationTimeInternal(StringView path) {
    PathStat st;
    if (pathStat(path, &st)) {
        return {};
    }
    return st.st_mtim.tv_sec * 1000000ll + st.st_mtim.tv_nsec / 1000;
}

static inline __int32_t
major(__uint32_t _x)
{
    return (__int32_t)(((__uint32_t)_x >> 24) & 0xff);
}

static inline __int32_t
minor(__uint32_t _x)
{
    return (__int32_t)((_x) & 0xffffff);
}

static Optional<System::DiskKind> diskKind(const PathStat& st) {
#ifdef _WIN32

    auto volumeName = StringFormat(R"(\\?\%c:)", 'A' + st.st_dev);
    ScopedFileHandle volume(::CreateFileA(volumeName.c_str(), 0,
                                          FILE_SHARE_READ | FILE_SHARE_WRITE,
                                          NULL, OPEN_EXISTING, 0, NULL));
    if (!volume.valid()) {
        return {};
    }

    VOLUME_DISK_EXTENTS volumeDiskExtents;
    DWORD bytesReturned = 0;
    if ((!::DeviceIoControl(volume.get(), IOCTL_VOLUME_GET_VOLUME_DISK_EXTENTS,
                            NULL, 0, &volumeDiskExtents,
                            sizeof(volumeDiskExtents), &bytesReturned, NULL) &&
         ::GetLastError() != ERROR_MORE_DATA) ||
        bytesReturned != sizeof(volumeDiskExtents)) {
        return {};
    }
    if (volumeDiskExtents.NumberOfDiskExtents < 1) {
        return {};
    }

    auto deviceName =
            StringFormat(R"(\\?\PhysicalDrive%d)",
                         int(volumeDiskExtents.Extents[0].DiskNumber));
    ScopedFileHandle device(::CreateFileA(deviceName.c_str(), 0,
                                          FILE_SHARE_READ | FILE_SHARE_WRITE,
                                          NULL, OPEN_EXISTING, 0, NULL));
    if (!device.valid()) {
        return {};
    }

    STORAGE_PROPERTY_QUERY spqTrim;
    spqTrim.PropertyId = (STORAGE_PROPERTY_ID)StorageDeviceTrimProperty;
    spqTrim.QueryType = PropertyStandardQuery;
    DEVICE_TRIM_DESCRIPTOR dtd = {0};
    if (::DeviceIoControl(device.get(), IOCTL_STORAGE_QUERY_PROPERTY, &spqTrim,
                          sizeof(spqTrim), &dtd, sizeof(dtd), &bytesReturned,
                          NULL) &&
        bytesReturned == sizeof(dtd)) {
        // Some SSDs don't support TRIM, so this can't be a sign of an HDD.
        if (dtd.TrimEnabled) {
            return System::DiskKind::Ssd;
        }
    }

    bytesReturned = 0;
    STORAGE_PROPERTY_QUERY spqSeekP;
    spqSeekP.PropertyId = (STORAGE_PROPERTY_ID)StorageDeviceSeekPenaltyProperty;
    spqSeekP.QueryType = PropertyStandardQuery;
    DEVICE_SEEK_PENALTY_DESCRIPTOR dspd = {0};
    if (::DeviceIoControl(device.get(), IOCTL_STORAGE_QUERY_PROPERTY, &spqSeekP,
                          sizeof(spqSeekP), &dspd, sizeof(dspd), &bytesReturned,
                          NULL) &&
        bytesReturned == sizeof(dspd)) {
        return dspd.IncursSeekPenalty ? System::DiskKind::Hdd
                                      : System::DiskKind::Ssd;
    }

    // TODO: figure out how to issue this query when not admin and not opening
    //  disk for write access.
#if 0
    bytesReturned = 0;

    // This struct isn't in the MinGW distribution headers.
    struct ATAIdentifyDeviceQuery {
        ATA_PASS_THROUGH_EX header;
        WORD data[256];
    };
    ATAIdentifyDeviceQuery id_query = {};
    id_query.header.Length = sizeof(id_query.header);
    id_query.header.AtaFlags = ATA_FLAGS_DATA_IN;
    id_query.header.DataTransferLength = sizeof(id_query.data);
    id_query.header.TimeOutValue = 5;  // Timeout in seconds
    id_query.header.DataBufferOffset =
            offsetof(ATAIdentifyDeviceQuery, data[0]);
    id_query.header.CurrentTaskFile[6] = 0xec;  // ATA IDENTIFY DEVICE
    if (::DeviceIoControl(device.get(), IOCTL_ATA_PASS_THROUGH, &id_query,
                          sizeof(id_query), &id_query, sizeof(id_query),
                          &bytesReturned, NULL) &&
        bytesReturned == sizeof(id_query)) {
        // Index of nominal media rotation rate
        // SOURCE:
        // http://www.t13.org/documents/UploadedDocuments/docs2009/d2015r1a-ATAATAPI_Command_Set_-_2_ACS-2.pdf
        //          7.18.7.81 Word 217
        // QUOTE: Word 217 indicates the nominal media rotation rate of the
        // device and is defined in table:
        //          Value           Description
        //          --------------------------------
        //          0000h           Rate not reported
        //          0001h           Non-rotating media (e.g., solid state
        //                          device)
        //          0002h-0400h     Reserved
        //          0401h-FFFEh     Nominal media rotation rate in rotations per
        //                          minute (rpm) (e.g., 7 200 rpm = 1C20h)
        //          FFFFh           Reserved
        unsigned rate = id_query.data[217];
        if (rate == 1) {
            return System::DiskKind::Ssd;
        } else if (rate >= 0x0401 && rate <= 0xFFFE) {
            return System::DiskKind::Hdd;
        }
    }
#endif

#elif defined __linux__

    // Parse /proc/partitions to find the corresponding device
    std::ifstream in("/proc/partitions");
    if (!in) {
        return {};
    }

    const auto maj = major(st.st_dev);
    const auto min = minor(st.st_dev);

    std::string line;
    std::string devName;

    std::unordered_set<std::string> devices;

    while (std::getline(in, line)) {
        unsigned curMaj, curMin;
        unsigned long blocks;
        char name[1024];
        if (sscanf(line.c_str(), "%u %u %lu %1023s", &curMaj, &curMin, &blocks,
                   name) == 4) {
            devices.insert(name);
            if (curMaj == maj && curMin == min) {
                devName = name;
                break;
            }
        }
    }
    if (devName.empty()) {
        return {};
    }
    in.close();

    if (maj == 8) {
        // get rid of the partition number for block devices.
        while (!devName.empty() && isdigit(devName.back())) {
            devName.pop_back();
        }
        if (devices.find(devName) == devices.end()) {
            return {};
        }
    }

    // Now, having a device name, let's parse
    // /sys/block/%device%X/queue/rotational to get the result.
    auto sysPath = StringFormat("/sys/block/%s/queue/rotational", devName);
    in.open(sysPath.c_str());
    if (!in) {
        return {};
    }
    char isRotational = 0;
    if (!(in >> isRotational)) {
        return {};
    }
    if (isRotational == '0') {
        return System::DiskKind::Ssd;
    } else if (isRotational == '1') {
        return System::DiskKind::Hdd;
    }

#else

    return nativeDiskKind(st.st_dev);

#endif

    // Sill got no idea.
    return {};
}

Optional<System::DiskKind> System::diskKindInternal(StringView path) {
    PathStat stat;
    if (pathStat(path, &stat)) {
        return {};
    }
    return android::base::diskKind(stat);
}

Optional<System::DiskKind> System::diskKindInternal(int fd) {
    PathStat stat;
    if (fdStat(fd, &stat)) {
        return {};
    }
    return android::base::diskKind(stat);
}

// static
void System::addLibrarySearchDir(StringView path) {
    System* system = System::get();
    const char* varName = kLibrarySearchListEnvVarName;

    std::string libSearchPath = system->envGet(varName);
    if (libSearchPath.size()) {
        libSearchPath =
                StringFormat("%s%c%s", path, kPathSeparator, libSearchPath);
    } else {
        libSearchPath = path;
    }
    system->envSet(varName, libSearchPath);
}

// static
std::string System::findBundledExecutable(StringView programName) {
    System* const system = System::get();
    const std::string executableName = PathUtils::toExecutableName(programName);

    // first, try the root launcher directory
    std::vector<std::string> pathList = {system->getLauncherDirectory(),
                                         executableName};
    std::string executablePath = PathUtils::recompose(pathList);
    if (system->pathIsFile(executablePath)) {
        return executablePath;
    }

    // it's not there - let's try the 'bin/' subdirectory
    assert(pathList.size() == 2);
    assert(pathList[1] == executableName.c_str());
    pathList[1] = kBinSubDir;
    pathList.push_back(executableName);
    executablePath = PathUtils::recompose(pathList);
    if (system->pathIsFile(executablePath)) {
        return executablePath;
    }

#if defined(_WIN32) && defined(__x86_64)
    // On Windows we don't have a x64 version e2fsprogs, so let's try
    // 32-bit directory if 64-bit lookup failed
    assert(pathList[1] == kBinSubDir);
    pathList[1] = kBin32SubDir;
    executablePath = PathUtils::recompose(pathList);
    if (system->pathIsFile(executablePath)) {
        return executablePath;
    }
#endif

    return std::string();
}

// static
int System::freeRamMb() {
    auto usage = get()->getMemUsage();
    uint64_t freePhysMb = usage.avail_phys_memory / (1024ULL * 1024ULL);
    return freePhysMb;
}

// static
bool System::isUnderMemoryPressure(int* freeRamMb_out) {
    uint64_t currentFreeRam = freeRamMb();

    if (freeRamMb_out) {
        *freeRamMb_out = currentFreeRam;
    }

    return currentFreeRam < kMemoryPressureLimitMb;
}

// static
bool System::isUnderDiskPressure(StringView path, System::FileSize* freeDisk) {
    System::FileSize availableSpace;
    bool success = System::get()->pathFreeSpace(path,
                                                &availableSpace);
    if (success && availableSpace < kDiskPressureLimitBytes) {
        if (freeDisk) *freeDisk = availableSpace;
        return true;
    }

    return false;
}

std::string toString(OsType osType) {
    switch (osType) {
    case OsType::Windows:
        return "Windows";
    case OsType::Linux:
        return "Linux";
    case OsType::Mac:
        return "Mac";
    default:
        return "Unknown";
    }
}

}  // namespace base
}  // namespace android
