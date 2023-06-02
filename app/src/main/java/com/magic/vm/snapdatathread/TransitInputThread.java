package com.magic.vm.snapdatathread;

import android.net.LocalServerSocket;
import android.net.LocalSocket;
import android.util.Log;
import android.view.KeyEvent;
import android.view.MotionEvent;

import com.magic.vm.boxutil.SocketUtil;
import com.magic.vm.boxutil.ThreadUtil;
import com.magic.vm.cpputil.InputEventFun;
import com.magic.vm.entity.TransitPathConstant;

import java.io.DataInputStream;
import java.io.DataOutputStream;
import java.util.ArrayList;
import java.util.Arrays;

public class TransitInputThread extends Thread {

    private final static String TAG = "xxx_input";
    private DataInputStream inputStream;
    private DataOutputStream outputStream;
    private final ArrayList<byte[]> dataList = new ArrayList<>();
    private final Object lock_array = new Object();
    private LocalServerSocket localServerSocket;

    @Override
    public void run() {
        startServer();
        ThreadUtil.runOnNewThread(() -> {
            while (true) {
                try {
                    byte[] bytes = new byte[1024];
                    int read = inputStream.read(bytes);
                    bytes = Arrays.copyOf(bytes, read);
                    Log.d(TAG, "run: read size = " + read + ", info:" + Arrays.toString(bytes));
                } catch (Throwable e) {
                    Log.e(TAG, "error: " + e.getLocalizedMessage());
                    SocketUtil.close(inputStream);
                    SocketUtil.close(outputStream);
                    inputStream = null;
                    outputStream = null;
                    startServer();
                }
            }
        });
        postInputEvent();
    }

    public void postEvent(MotionEvent event, int screenWidth, int screenHeight) {
        postData(InputEventFun.formatEvent(event, screenWidth, screenHeight));
    }

    public void postEvent(KeyEvent event, boolean isDown) {
        postData(InputEventFun.formatKeyEvent(event, isDown));
    }

    public void postData(byte[] data) {
        if (data == null || data.length == 0)
            return;
        if (inputStream == null || outputStream == null) {
            return;
        }
        synchronized (lock_array) {
            dataList.add(data);
        }
    }

    private void postInputEvent() {
        while (true) {
            byte[] data = null;
            synchronized (lock_array) {
                if (dataList.size() > 0) {
                    data = dataList.get(0);
                    dataList.remove(0);
                }
            }
            if (outputStream == null || data == null || data.length == 0) continue;
            try {
                outputStream.write(data);
            } catch (Throwable e) {
                Log.e(TAG, "postData error:", e);
                SocketUtil.close(outputStream);
                outputStream = null;
                startServer();
                dataList.clear();
            }
        }
    }

    private void startServer() {
        do {
            try {
                if (localServerSocket == null)
                    localServerSocket = new LocalServerSocket(
                            TransitPathConstant.UNIX_INPUT_EVENT_HUB + TransitPathConstant.UNIX_SUFFIX);
                Log.e(TAG, "EventHub startServer: wait accept");
                LocalSocket accept = localServerSocket.accept();
                Log.e(TAG, "EventHub startServer: accept");
                inputStream = new DataInputStream(accept.getInputStream());
                outputStream = new DataOutputStream(accept.getOutputStream());
                break;
            } catch (Throwable e) {
                Log.e(TAG, "EventHub startServer error: " + e.getLocalizedMessage());
                ThreadUtil.sleep(3);
            }
        } while (true);
    }
}
