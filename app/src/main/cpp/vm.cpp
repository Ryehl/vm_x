#include <jni.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h>
#include <string>
#include <vector>
#include <dlfcn.h>
#include <errno.h>

#include "log.h"
#include "guard.h"

static std::string getRealPath(const char *origPath) {
    int fd = open(origPath, O_RDONLY);
    if (fd < 0) {
        return origPath;
    }

    std::vector<char> procSelfFd(PATH_MAX);
    sprintf(procSelfFd.data(), "/proc/self/fd/%d", fd);

    std::vector<char> buf(PATH_MAX);
    if (readlink(procSelfFd.data(), buf.data(), buf.size()) < 0) {
        close(fd);
        return origPath;
    }

    close(fd);
    return buf.data();
}

extern "C"
JNIEXPORT void JNICALL
Java_com_magic_vm_Vm_start(JNIEnv *env, jclass clazz, jstring _rootPath) {
    const char *rootPath = env->GetStringUTFChars(_rootPath, JNI_FALSE);
    ReleaseStringUTFChars_On_Exist(_rootPath, rootPath);

    pid_t pid = fork();
    if (pid < 0) {
        LOGE("xxx Failed to fork, errno:%d", errno);
        return;
    }

    if (pid == 0) {
        LOGE("xxx child pid:%d", getpid(), getuid());

        std::string rp = getRealPath(rootPath);
        chdir(rp.c_str());
        setenv("BVM_ROOT", rp.c_str(), 1);

        std::string initPath = rp + "/init";
        execl(initPath.c_str(), initPath.c_str(), nullptr);
    } else {
        LOGE("xxx parent pid:%d", getpid(), getuid());
    }
}
