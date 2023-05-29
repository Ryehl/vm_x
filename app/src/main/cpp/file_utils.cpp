#include <jni.h>
#include <unistd.h>
#include <sys/stat.h>
#include "guard.h"

extern "C"
JNIEXPORT jint JNICALL
Java_com_magic_vm_util_FileUtils_chmod(JNIEnv *env, jclass clazz, jstring _path, jint mode) {
    const char *path = env->GetStringUTFChars(_path, JNI_FALSE);
    ReleaseStringUTFChars_On_Exist(_path, path);
    return chmod(path, mode);
}

extern "C"
JNIEXPORT jint JNICALL
Java_com_magic_vm_util_FileUtils_symlink(JNIEnv *env, jclass clazz, jstring _path1, jstring _path2) {
    const char *path1 = env->GetStringUTFChars(_path1, JNI_FALSE);
    ReleaseStringUTFChars_On_Exist(_path1, path1);
    const char *path2 = env->GetStringUTFChars(_path2, JNI_FALSE);
    ReleaseStringUTFChars_On_Exist(_path2, path2);
    return symlink(path1, path2);
}