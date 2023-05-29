#include <jni.h>

//
// Created by Xaoyv on 2023/4/7.
//
#include <android/native_window.h>
#include <android/native_window_jni.h>
#include <dlfcn.h>
#include "bm_gles/bm_opengles.h"

ANativeWindow *mWindow = NULL;
int mWidth = 0;
int mHeight = 0;

extern "C"
JNIEXPORT void JNICALL
Java_com_magic_vm_MainActivity_bm_1surfaceCreated(JNIEnv *env, jclass clazz, jobject surface) {
    if (mWindow) {
        ANativeWindow_release(mWindow);
        mWindow = nullptr;
    }
    mWindow = ANativeWindow_fromSurface(env, surface);
    const float dpr = 1.0;
    android_showOpenglesWindow(mWindow, 0 /*x*/, 0/*y*/,
                               mWidth ? mWidth : 1080/*ww=1080*/,
                               mHeight ? mHeight : 1920/*wh=1920*/,
                               mWidth ? mWidth : 1080,
                               mHeight ? mHeight : 1920,// framebuffer width, height
                               dpr/*dpr=1*/, 0/*rotation=90*/, false /*deleteExisting=changed*/);
}

extern "C"
JNIEXPORT void JNICALL
Java_com_magic_vm_MainActivity_bm_1surfaceChanged(JNIEnv *env, jclass clazz, jobject surface,
                                                  jint format, jint width, jint height) {
//    if (!mWindow)
//        mWindow = ANativeWindow_fromSurface(env, surface);
    const float dpr = 1.0;
    android_showOpenglesWindow(mWindow, 0 /*x*/, 0/*y*/,
                               width/*ww=1080*/, height/*wh=1920*/,
                               width, height,// framebuffer width, height
                               dpr/*dpr=1*/, 0/*rotation=90*/, true /*deleteExisting=changed*/);
}

extern "C"
JNIEXPORT void JNICALL
Java_com_magic_vm_MainActivity_bm_1surfaceRedraw(JNIEnv *env, jclass clazz, jobject surface) {
    //post
    android_redrawOpenglesWindow();
}

extern "C"
JNIEXPORT void JNICALL
Java_com_magic_vm_MainActivity_bm_1surfaceDestroyed(JNIEnv *env, jclass clazz, jobject surface) {
    android_hideOpenglesWindow();
    mWindow = nullptr;
}

extern "C"
JNIEXPORT void JNICALL
Java_com_magic_vm_MainActivity_bm_1startRenderThread(JNIEnv *env, jclass clazz, jint width,
                                                     jint height) {
    int major, minor;
    mWidth = width;mHeight = height;

    android_startOpenglesRenderer(width, height,
                                  false, 28,
                                  &major, &minor);
}

extern "C"
JNIEXPORT void JNICALL
Java_com_magic_vm_MainActivity_bm_1initRenderLib(JNIEnv *env, jclass clazz) {
    android_initOpenglesEmulation();
}
extern "C"
JNIEXPORT void JNICALL
Java_com_magic_vm_MainActivity_bm_1startTcp(JNIEnv *env, jclass clazz) {
    android_startTcpListener();
}