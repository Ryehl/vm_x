package com.magic.vm.boxutil;

import android.os.Parcel;
import android.os.Parcelable;
import android.os.SystemClock;
import android.util.Log;
import android.view.KeyEvent;
import android.view.MotionEvent;

import java.lang.reflect.Field;

public class EventParcelableUtil {

    private static final long NS_PER_MS = 1000000;//1,000,000  1,000,000

    private static int screenWidth = 1080;

    public static void setScreenWidth(int screenWidth) {
        EventParcelableUtil.screenWidth = screenWidth;
    }

    public static byte[] marshall(Parcelable event, int top, float scale, boolean isVer) {
        Parcel parcel = Parcel.obtain();

        parcel.setDataPosition(0);

        if (event instanceof KeyEvent) {
            writeToParcel((KeyEvent) event, parcel);
        }
        if (event instanceof MotionEvent) {
            writeToParcel((MotionEvent) event, parcel, top, scale, isVer);
        }
        byte[] bytes = parcel.marshall();
        parcel.recycle();
        return bytes;
    }

    public static long getBootTime() {
        return System.currentTimeMillis() - SystemClock.elapsedRealtime();
    }

    public static void writeToParcel(MotionEvent motionEvent, Parcel out, int top, float scale, boolean isVer) {
        int pointerCount = motionEvent.getPointerCount();
        int sampleCount = motionEvent.getHistorySize() + 1;

        out.writeInt(1);

        out.writeInt(pointerCount);
        out.writeInt(sampleCount);

        out.writeInt(motionEvent.getDeviceId());
        out.writeInt(motionEvent.getSource());
        out.writeInt(motionEvent.getAction());
        out.writeInt(motionEvent.getActionButton());
        out.writeInt(motionEvent.getFlags());
        out.writeInt(motionEvent.getEdgeFlags());
        out.writeInt(motionEvent.getMetaState());
        out.writeInt(motionEvent.getButtonState());

        //change xy
        float y = motionEvent.getY();
        y = (y - top) * scale;
        if (isVer) {
            out.writeFloat(motionEvent.getX() * scale);
            out.writeFloat(y);
        } else {
            out.writeFloat(y);
            out.writeFloat(screenWidth - motionEvent.getX() * scale);
        }

        if (isVer) {
            out.writeFloat(motionEvent.getXPrecision());
            out.writeFloat(motionEvent.getYPrecision());
        } else {
            out.writeFloat(motionEvent.getYPrecision());
            out.writeFloat(screenWidth - motionEvent.getXPrecision());
        }

        out.writeLong(motionEvent.getDownTime() * NS_PER_MS);

        MotionEvent.PointerProperties properties = new MotionEvent.PointerProperties();
        for (int i = 0; i < pointerCount; i++) {
            motionEvent.getPointerProperties(i, properties);
            out.writeInt(properties.id);
            out.writeInt(properties.toolType);
        }

        MotionEvent.PointerCoords pointerCoords = new MotionEvent.PointerCoords();
        for (int i = 0; i < sampleCount; i++) {

            if (i == sampleCount - 1) {
                out.writeLong(motionEvent.getEventTime() * NS_PER_MS);
            } else {
                out.writeLong(motionEvent.getHistoricalEventTime(i) * NS_PER_MS);
            }

            for (int j = 0; j < pointerCount; j++) {
                if (i == sampleCount - 1) {
                    motionEvent.getPointerCoords(j, pointerCoords);
                } else {
                    motionEvent.getHistoricalPointerCoords(j, i, pointerCoords);
                }
                try {
                    Field mPackedAxisBits = pointerCoords.getClass().getDeclaredField("mPackedAxisBits");
                    mPackedAxisBits.setAccessible(true);
                    long bits = mPackedAxisBits.getLong(pointerCoords);

                    Field mPackedAxisValues = pointerCoords.getClass().getDeclaredField("mPackedAxisValues");
                    mPackedAxisValues.setAccessible(true);
                    float[] declaredAnnotations = (float[]) mPackedAxisValues.get(pointerCoords);

                    out.writeLong(bits);
                    if (declaredAnnotations != null) {
                        for (float declaredAnnotation : declaredAnnotations) {
                            out.writeFloat(declaredAnnotation);
                        }
                    }
                } catch (Throwable e) {
                    Log.e("---", "writeToParcel: error:", e);
                }

            }

        }
    }

    public static void writeToParcel(KeyEvent keyEvent, Parcel out) {
        out.writeInt(2);

        out.writeInt(keyEvent.getDeviceId());
        out.writeInt(keyEvent.getSource());
        out.writeInt(keyEvent.getAction());
        out.writeInt(keyEvent.getKeyCode());
        out.writeInt(keyEvent.getRepeatCount());

        out.writeInt(keyEvent.getMetaState());
        out.writeInt(keyEvent.getScanCode());
        out.writeInt(keyEvent.getFlags());
        out.writeLong(keyEvent.getDownTime());
        out.writeLong(keyEvent.getEventTime());
    }

    public static Parcel unmarshall(byte[] bytes) {
        Parcel parcel = Parcel.obtain();
        parcel.unmarshall(bytes, 0, bytes.length);
        parcel.setDataPosition(0); // This is extremely important!
        return parcel;
    }

    public static <T> T unmarshall(byte[] bytes, Parcelable.Creator<T> creator) {
        Parcel parcel = unmarshall(bytes);
        T result = creator.createFromParcel(parcel);
        parcel.recycle();
        return result;
    }
}
