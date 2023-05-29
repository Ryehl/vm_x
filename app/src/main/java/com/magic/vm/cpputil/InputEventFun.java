package com.magic.vm.cpputil;

import android.util.Log;
import android.view.KeyEvent;
import android.view.MotionEvent;

import java.util.Arrays;

public class InputEventFun {
    private static final int SQ = 1 << 15;//32768

    private static final int SIZE_INPUT_EVENT = 24;
    private static final int SIZEOF_KEY_RESULT = 2 * 24;

    public static byte[] formatEvent(MotionEvent event, int screenWidth, int screenHeight) {
        int pointerCount = event.getPointerCount();
        int[] data = new int[pointerCount * 3];
        for (int i = 0; i < pointerCount; i++) {
            data[i * 3] = event.getPointerId(i);
            data[i * 3 + 1] = (int) (event.getX(i) * SQ / screenWidth);
            data[i * 3 + 2] = (int) (event.getY(i) * SQ / screenHeight);
        }
        byte[] result = new byte[SIZE_INPUT_EVENT * 256];
        int size = formatMotionData(data, pointerCount, event.getAction(), result);
        result = Arrays.copyOf(result, size);
        return result;
    }

    public static byte[] formatKeyEvent(KeyEvent event, boolean isDown) {
        byte[] bytes = new byte[SIZEOF_KEY_RESULT];
        int i = formatKeyEventData(event.getKeyCode(), event.getAction() == KeyEvent.ACTION_DOWN, bytes);
        if (i == 0){
            return null;
        }
        return bytes;
    }

    public static native int formatMotionData(int[] data, int count, int action, byte[] result);

    /**
     * @param result 8byte
     * @return state 8 / -1
     */
    public static native int formatKeyEventData(int keyCode, boolean isDown, byte[] result);
}
