#include <jni.h>
#include <cstdlib>
#include <cstring>
#include <ctime>

//
// Created by xaoyv on 2023/5/19.
//

static int kMaxChannelValue = 262143;//0x3FFFF


#define RETURN_ARGB 0

#if RETURN_ARGB
//argb
uint32_t YUV2RGB(jint y, jint u, jint v) {
#else
//abgr
uint32_t YUV2BGR(jint y, jint u, jint v) {
#endif
    // Adjust and check YUV values
    y = (y - 16) < 0 ? 0 : (y - 16);
    u -= 128;
    v -= 128;

    // This is the floating point equivalent. We do the conversion in integer
    // because some Android devices do not have floating point in hardware.
    // nR = (int)(1.164 * nY + 2.018 * nU);
    // nG = (int)(1.164 * nY - 0.813 * nV - 0.391 * nU);
    // nB = (int)(1.164 * nY + 1.596 * nV);
    int y1192 = 1192 * y;
    int r = (y1192 + 1634 * v);
    int g = (y1192 - 833 * v - 400 * u);
    int b = (y1192 + 2066 * u);

    // Clipping RGB values to be inside boundaries [ 0 , kMaxChannelValue ]
    r = r > kMaxChannelValue ? kMaxChannelValue : (r < 0 ? 0 : r);
    g = g > kMaxChannelValue ? kMaxChannelValue : (g < 0 ? 0 : g);
    b = b > kMaxChannelValue ? kMaxChannelValue : (b < 0 ? 0 : b);

#if RETURN_ARGB
    //argb
    return 0xff000000
           | ((r << 6) & 0xff0000)
           | ((g >> 2) & 0xff00)
           | ((b >> 10) & 0xff);
#else
    //abgr
    return 0xff000000
           | ((b << 6) & 0xff0000)
           | ((g >> 2) & 0xff00)
           | ((r >> 10) & 0xff);
#endif
}


extern "C"
JNIEXPORT void JNICALL
Java_com_magic_vm_cpputil_CameraFun_intArr2byteArr(JNIEnv *env, jclass clazz, jintArray src,
                                                   jint intLen, jbyteArray out) {
    int len_out = intLen * 4;
    jbyte *b_arr = (jbyte *) malloc(len_out);
    jint *source = env->GetIntArrayElements(src, NULL);
    memcpy(b_arr, source, len_out);
    env->SetByteArrayRegion(out, 0, len_out, b_arr);
    free(b_arr);
}
extern "C"
JNIEXPORT void JNICALL
Java_com_magic_vm_cpputil_CameraFun_convertYUV420ToARGB8888(JNIEnv *env, jclass clazz,
                                                            jbyteArray y_data, jbyteArray u_data,
                                                            jbyteArray v_data, jint width,
                                                            jint height, jint y_row_stride,
                                                            jint uv_row_stride,
                                                            jint uv_pixel_stride, jbyteArray out) {
    jbyte *yData = env->GetByteArrayElements(y_data, NULL);
    jbyte *uData = env->GetByteArrayElements(u_data, NULL);
    jbyte *vData = env->GetByteArrayElements(v_data, NULL);
    uint32_t *rgb = (uint32_t *) malloc(sizeof(jint) * height * width);
    int yp = 0;
    for (int j = 0; j < height; j++) {
        int pY = y_row_stride * j;
        int pUV = uv_row_stride * (j >> 1);

        for (int i = 0; i < width; i++) {
            int uv_offset = pUV + (i >> 1) * uv_pixel_stride;


#if RETURN_ARGB
            rgb[yp++] = YUV2RGB(0xff & yData[pY + i], 0xff & uData[uv_offset],
                                0xff & vData[uv_offset]);
#else
            rgb[yp++] = YUV2BGR(0xff & yData[pY + i], 0xff & uData[uv_offset],
                                0xff & vData[uv_offset]);
#endif

        }
    }
    env->SetByteArrayRegion(out, 0, sizeof(jint) * height * width, (jbyte *) rgb);
    free(rgb);
}
extern "C"
JNIEXPORT jlong JNICALL
Java_com_magic_vm_cpputil_CameraFun_getCameraFrameTime(JNIEnv *env, jclass clazz) {
    struct timespec t;
    t.tv_sec = t.tv_nsec = 0;
    clock_gettime(CLOCK_MONOTONIC, &t);
    int64_t result = int64_t(t.tv_sec) * 1000000000LL + t.tv_nsec;
    return (jlong) result + (1000LL * 30LL);
}