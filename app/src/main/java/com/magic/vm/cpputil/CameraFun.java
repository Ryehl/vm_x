package com.magic.vm.cpputil;

public class CameraFun {
    static {
        System.loadLibrary("camera");
    }
    public static native void intArr2byteArr(int[] src, int length, byte[] out);

    public static native void convertYUV420ToARGB8888(
            byte[] yData,
            byte[] uData,
            byte[] vData,
            int width,
            int height,
            int yRowStride,
            int uvRowStride,
            int uvPixelStride,
            byte[] out);

    public static native long getCameraFrameTime();
}
