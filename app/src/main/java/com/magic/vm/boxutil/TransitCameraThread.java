package com.magic.vm.boxutil;

import android.Manifest;
import android.content.Context;
import android.content.pm.PackageManager;
import android.graphics.ImageFormat;
import android.graphics.SurfaceTexture;
import android.hardware.camera2.CameraAccessException;
import android.hardware.camera2.CameraCaptureSession;
import android.hardware.camera2.CameraCharacteristics;
import android.hardware.camera2.CameraDevice;
import android.hardware.camera2.CameraManager;
import android.hardware.camera2.CaptureRequest;
import android.hardware.camera2.params.StreamConfigurationMap;
import android.media.Image;
import android.media.ImageReader;
import android.net.LocalSocket;
import android.os.Handler;
import android.os.HandlerThread;
import android.util.Log;
import android.util.Size;

import androidx.annotation.NonNull;
import androidx.core.app.ActivityCompat;

import com.magic.vm.cpputil.CameraFun;

import java.io.DataInputStream;
import java.io.DataOutputStream;
import java.io.IOException;
import java.nio.ByteBuffer;
import java.util.Collections;

public class TransitCameraThread extends Thread {
    private static final String TAG = "TransitCamera";
    //connect
    private final LocalSocket accept;
    //camera
    private final Context mContext;
    private DataOutputStream outputStream;
    private DataInputStream inputStream;
    private String cameraId;
    private CameraManager manager;
    private Handler backgroundHandler;
    private CameraDevice cameraDevice;
    private ImageReader reader;
    private CameraCaptureSession captureSession;
    private CaptureRequest previewRequest;
    private CaptureRequest.Builder captureRequest = null;

    //image
    private final byte[][] yuvBytes = new byte[3][];
    private byte[] rgbBytes;

    private boolean isRunning;

    public TransitCameraThread(Context context, LocalSocket accept) {
        this.mContext = context.getApplicationContext();
        this.accept = accept;
    }

    @Override
    public void run() {
        HandlerThread backgroundThread = new HandlerThread("ImageListener");
        backgroundThread.start();
        backgroundHandler = new Handler(backgroundThread.getLooper());
        try {
            outputStream = new DataOutputStream(accept.getOutputStream());
            inputStream = new DataInputStream(accept.getInputStream());
            manager = (CameraManager) mContext.getSystemService(Context.CAMERA_SERVICE);
        } catch (Throwable e) {
            Log.e(TAG, "run error: ", e);
            SocketUtil.close(outputStream);
            SocketUtil.close(inputStream);
            SocketUtil.close(accept);
            return;
        }
        isRunning = true;
        doDecode();
    }

    private void sendLH(int value) {
        try {
            for (int i = 0; i < 4; i++) {
                outputStream.writeByte((byte) ((value >> (8 * i)) & 0xFF));
            }
        } catch (Throwable e) {
            Log.e(TAG, "sendLH error: ", e);
        }
    }

    private void doDecode() {
        while (isRunning) {
            try {
                byte[] bytes = new byte[4];
                inputStream.readFully(bytes);
                int code = ByteUtil.byteArr2IntVal(bytes, 0);
                inputStream.readFully(bytes);
                int len = ByteUtil.byteArr2IntVal(bytes, 0);
                byte[] content = new byte[len];
                inputStream.readFully(content);
                switch (code) {
                    case CameraCode.CAMERA_FAKE: {
//                        Log.e(TAG, "CAMERA_FAKE: ");
                        //no fake camera
                        sendLH(0);
                    }
                    break;
                    case CameraCode.CAMERA_LIST: {
//                        Log.e(TAG, "CAMERA_LIST: ");
                        String[] cameraIdList = null;
                        try {
                            cameraIdList = manager.getCameraIdList();
                        } catch (CameraAccessException e) {
                            Log.e(TAG, "doDecode(manager.getCameraIdList): ", e);
                        }
                        StringBuilder stringBuilder = new StringBuilder();
                        if (cameraIdList != null && cameraIdList.length > 0) {
                            for (int i = 0; i < cameraIdList.length; i++) {
                                String s = cameraIdList[i];
                                StringBuilder sb_temp = new StringBuilder();
                                //name - id
                                sb_temp.append("name=").append(s);

                                //dims - size
                                CameraCharacteristics cameraCharacteristics;
                                try {
                                    cameraCharacteristics = manager.getCameraCharacteristics(s);
                                } catch (CameraAccessException e) {
                                    Log.w(TAG, "doDecode (getCameraCharacteristics) error: ", e);
                                    continue;
                                }

//                                Size[] sizes_jpeg = cameraCharacteristics.get(CameraCharacteristics.JPEG_AVAILABLE_THUMBNAIL_SIZES);
                                StreamConfigurationMap streamConfigurationMap = cameraCharacteristics.get(CameraCharacteristics.SCALER_STREAM_CONFIGURATION_MAP);
                                if (streamConfigurationMap == null) continue;
                                Size[] sizes_jpeg = streamConfigurationMap.getOutputSizes(SurfaceTexture.class);
                                if (sizes_jpeg == null || sizes_jpeg.length == 0) {
                                    Log.e(TAG, "get Size: sizes_jpeg: null");
                                    continue;
                                }
//                                Log.e(TAG, "get Size: sizes_jpeg: size: " + sizes_jpeg.length + ", value: " + Arrays.toString(sizes_jpeg));
                                sb_temp.append(" ").append("framedims=");
                                for (Size size : sizes_jpeg) {
                                    if (size.getHeight() < 1 || size.getWidth() < 1) continue;
                                    sb_temp.append(size.getWidth()).append("x").append(size.getHeight()).append(',');
                                }

                                //dir - back, front, ext
                                sb_temp.append(" ").append("dir=");
                                Integer facing = cameraCharacteristics.get(CameraCharacteristics.LENS_FACING);
                                if (facing == null) {
                                    Log.e(TAG, "startListener: facing = null");
                                    continue;
                                }
                                if (facing == CameraCharacteristics.LENS_FACING_FRONT) {
                                    sb_temp.append("front");
                                } else if (facing == CameraCharacteristics.LENS_FACING_BACK) {
                                    sb_temp.append("back");
                                } else if (facing == CameraCharacteristics.LENS_FACING_EXTERNAL) {
                                    sb_temp.append("external");
                                } else {
                                    continue;
                                }
                                sb_temp.append("\n");
                                stringBuilder.append(sb_temp);
                            }
                        }
//                        Log.e(TAG, "startListener: " + stringBuilder);
                        String info = stringBuilder.toString();
                        sendLH(info.length());
                        outputStream.writeBytes(info);
                    }
                    break;

                    //init
                    case CameraCode.CAMERA_INIT: {
                        String s = new String(content, 0, content.length - 1);
                        if (s.startsWith("name=")) {
                            cameraId = new String(content, 5/*"name=".length*/, content.length - 1 - 5);
                        }
                        Log.e(TAG, "CAMERA_INIT: name = " + cameraId);
                    }
                    break;
                    case CameraCode.CAMERA_CLOSE: {
                        Log.e(TAG, "CAMERA_CLOSE: ");
                        isRunning = false;
                        closeCamera();
                        closeSocket();
                    }
                    break;

                    case CameraCode.CAMERA_CONNECT: {
                        Log.e(TAG, "CAMERA_CONNECT: ");
                        //do noting?
                        if (cameraId == null) {
                            sendLH(-1);
                            Log.e(TAG, "doDecode: send -1");
                            break;
                        }
                        if (ActivityCompat.checkSelfPermission(mContext, Manifest.permission.CAMERA) != PackageManager.PERMISSION_GRANTED) {
                            sendLH(-2);
                            Log.e(TAG, "doDecode: send -2");
                            break;
                        }
                        try {
                            manager.openCamera(cameraId, new CameraDevice.StateCallback() {
                                @Override
                                public void onOpened(@NonNull CameraDevice camera) {
                                    cameraDevice = camera;
                                }

                                @Override
                                public void onDisconnected(@NonNull CameraDevice camera) {
                                    cameraDevice.close();
                                }

                                @Override
                                public void onError(@NonNull CameraDevice camera, int error) {
                                    cameraDevice.close();
                                    Log.e(TAG, "openCamera onError: " + error);
                                }
                            }, backgroundHandler);
                            sendLH(0);
                        } catch (Throwable e) {
                            Log.e(TAG, "doDecode: ", e);
                            sendLH(-3);
                        }
                    }
                    break;
                    case CameraCode.CAMERA_DISCONNECT: {
                        Log.e(TAG, "CAMERA_DISCONNECT: ");
                        closeCamera();
                    }
                    break;
                    case CameraCode.CAMERA_START: {
                        int pixel_format = ByteUtil.byteArr2IntVal(content, 0);
                        int width = ByteUtil.byteArr2IntVal(content, 4);
                        int height = ByteUtil.byteArr2IntVal(content, 4 * 2);
                        Log.e(TAG, "CAMERA_START: pixel_format = " + logFormatStr(pixel_format) + "(" + pixel_format + ")" + ", width = " + width + ", height = " + height);
                        if (cameraId == null) {
                            sendLH(-1);
                            break;
                        }
                        if (cameraDevice == null) {
                            sendLH(-2);
                            break;
                        }
                        try {
                            captureRequest = cameraDevice.createCaptureRequest(CameraDevice.TEMPLATE_PREVIEW);
                        } catch (CameraAccessException e) {
                            Log.e(TAG, "doDecode: ", e);
                            sendLH(-3);
                            break;
                        }
                        if (captureRequest == null) {
                            sendLH(-4);
                            break;
                        }
                        if (reader != null) {
                            reader.close();
                        }
                        reader = ImageReader.newInstance(width, height, ImageFormat.YUV_420_888, 4);
                        reader.setOnImageAvailableListener(reader -> {
//                            Log.e(TAG, "doDecode: onImageAvailable ");
                            Image image = reader.acquireLatestImage();
                            if (image == null) {
                                return;
                            }
                            Image.Plane[] planes = image.getPlanes();//2 * w*h
                            fillBytes(planes, yuvBytes);//[0]: 76800, [1]: 38399, [2]: 38399
                            if (rgbBytes == null) {
                                rgbBytes = new byte[width * height * 4];
                            }
                            CameraFun.convertYUV420ToARGB8888(yuvBytes[0], yuvBytes[1], yuvBytes[2], width, height, planes[0].getRowStride(), planes[1].getRowStride(), planes[1].getPixelStride(), rgbBytes);
                            image.close();
                        }, backgroundHandler);
                        captureRequest.addTarget(reader.getSurface());
                        CameraCaptureSession.StateCallback callback = new CameraCaptureSession.StateCallback() {
                            @Override
                            public void onConfigured(@NonNull CameraCaptureSession session) {
                                captureSession = session;
                                captureRequest.set(CaptureRequest.CONTROL_AF_MODE, CaptureRequest.CONTROL_AF_MODE_CONTINUOUS_PICTURE);
                                // Flash is automatically enabled when necessary.
                                captureRequest.set(CaptureRequest.CONTROL_AE_MODE, CaptureRequest.CONTROL_AE_MODE_ON_AUTO_FLASH);

                                // Finally, we start displaying the camera preview.
                                previewRequest = captureRequest.build();
                                try {
                                    captureSession.setRepeatingRequest(previewRequest, new CameraCaptureSession.CaptureCallback() {
                                    }, backgroundHandler);
                                } catch (CameraAccessException e) {
                                    Log.e(TAG, "onConfigured: ", e);
                                }
                            }

                            @Override
                            public void onConfigureFailed(@NonNull CameraCaptureSession session) {
                            }
                        };
                        try {
                            cameraDevice.createCaptureSession(Collections.singletonList(reader.getSurface()), callback, backgroundHandler);
                            sendLH(0);
                        } catch (CameraAccessException e) {
                            Log.e(TAG, "doDecode: ", e);
                            sendLH(-1);
                        }
                    }
                    break;
                    case CameraCode.CAMERA_STOP: {
                        Log.e(TAG, "CAMERA_STOP: ");
                        closeCamera();
                    }
                    break;
                    case CameraCode.CAMERA_FRAME: {
                        long vFrameSize = getLongFromByteArr(content, 0);
                        long pFrameSize = getLongFromByteArr(content, 8);
//                        float r_scale = Float.intBitsToFloat(ByteUtil.byteArr2IntVal(content, 16));
//                        float g_scale = Float.intBitsToFloat(ByteUtil.byteArr2IntVal(content, 16 + 4));
//                        float b_scale = Float.intBitsToFloat(ByteUtil.byteArr2IntVal(content, 16 + 4 * 2));
//                        float exporsure_dump = Float.intBitsToFloat(ByteUtil.byteArr2IntVal(content, 16 + 4 * 3));
                        int hasTime = ByteUtil.byteArr2IntVal(content, 16 + 4 * 3 + 4);
//                        Log.e(TAG, "CAMERA_FRAME: vFrameSize = " + vFrameSize
//                                + ", pFrameSize = " + pFrameSize
//                                + ", r_scale = " + r_scale
//                                + ", g_scale = " + g_scale
//                                + ", b_scale = " + b_scale
//                                + ", exporsure_dump = " + exporsure_dump
//                                + ", hasTime = " + hasTime
//                        );
                        if (vFrameSize > 0) {//115200=w*h*1.5 pixel_format = NV21(825382478), width = 320, height = 240
                            if (yuvBytes[0] == null) {
                                byte[] bytes1 = new byte[(int) vFrameSize];
                                outputStream.write(bytes1);
                            } else {
                                outputStream.write(yuvBytes[0]);
                                outputStream.write(yuvBytes[2]);
                                outputStream.write(new byte[]{'\0'});
                            }
                        }
                        if (pFrameSize > 0) {
                            if (rgbBytes != null) {
                                outputStream.write(rgbBytes, 0, (int) pFrameSize);
                                if (rgbBytes.length < pFrameSize) {
                                    outputStream.write(new byte[(int) pFrameSize - rgbBytes.length]);
                                }
                            } else {
                                outputStream.write(new byte[(int) pFrameSize]);
                            }
                        }
                        if (hasTime != 0) {
                            //cpp getSystemTime()
                            outputStream.write(long2byteArr(CameraFun.getCameraFrameTime()));
                        }
                    }
                    break;

                    default:
                        Log.e(TAG, "startListener: default--code = " + code);
                }
            } catch (IOException e) {
                Log.e(TAG, "startListener run error: ", e);
                closeCamera();
                closeSocket();
                break;
            }
        }
    }

    private long getLongFromByteArr(byte[] content, int start) {
        long result = 0;
        for (int i = start; i < start + 8 && i < content.length; ++i) {
            result |= (0xFFL & content[i]) << ((i - start) * 8);
        }
        return result;
    }

    private byte[] long2byteArr(long l) {
        byte[] bytes = new byte[8];
        for (int i = 0; i < bytes.length; ++i) {
            bytes[i] = (byte) ((l >> (i * 8)) & ((byte) 0xFF));
        }
        return bytes;
    }

    private void fillBytes(final Image.Plane[] planes, final byte[][] yuvBytes) {
        // Because of the variable row stride it's not possible to know in
        // advance the actual necessary dimensions of the yuv planes.
        for (int i = 0; i < planes.length; ++i) {
            final ByteBuffer buffer = planes[i].getBuffer();
            if (yuvBytes[i] == null) {
//                Log.d(TAG, String.format("Initializing buffer %d at size %d", i, buffer.capacity()));
                yuvBytes[i] = new byte[buffer.capacity()];
            }
            buffer.get(yuvBytes[i]);
        }
    }

    private String logFormatStr(int value) {
        char a, r, g, b;
        a = (char) (value & 0xff);
        r = (char) ((value >> 8) & 0xff);
        g = (char) ((value >> 8 * 2) & 0xff);
        b = (char) ((value >> 8 * 3) & 0xff);
        return String.format("%c%c%c%c", a, r, g, b);
    }

    private void closeCamera() {
        if (captureSession != null) {
            captureSession.close();
        }
        captureSession = null;
        if (cameraDevice != null) {
            cameraDevice.close();
        }
        cameraDevice = null;
        if (reader != null) {
            reader.close();
        }
        reader = null;
        cameraId = null;
    }

    private void closeSocket() {
        SocketUtil.close(inputStream);
        SocketUtil.close(outputStream);
        SocketUtil.close(accept);
        inputStream = null;
        outputStream = null;
        isRunning = false;
    }

    private static class CameraCode {
        public static final int CAMERA_FAKE = 0x1;
        public static final int CAMERA_LIST = 0x2;

        public static final int CAMERA_INIT = 0x3;
        public static final int CAMERA_CLOSE = 0x4;

        public static final int CAMERA_CONNECT = 0x11;
        public static final int CAMERA_DISCONNECT = 0x12;
        public static final int CAMERA_START = 0x13;
        public static final int CAMERA_STOP = 0x14;
        public static final int CAMERA_FRAME = 0x15;
    }
}
