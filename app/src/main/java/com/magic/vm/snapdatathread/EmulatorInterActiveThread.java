package com.magic.vm.snapdatathread;

import android.net.LocalServerSocket;
import android.net.LocalSocket;
import android.util.Log;

import com.magic.vm.MainActivity;
import com.magic.vm.boxutil.SocketUtil;
import com.magic.vm.boxutil.ThreadUtil;
import com.magic.vm.entity.ActionConstant;
import com.magic.vm.entity.InterActiveEntity;
import com.magic.vm.entity.TransitPathConstant;

import java.io.DataInputStream;
import java.io.DataOutputStream;
import java.io.IOException;
import java.util.ArrayList;

public class EmulatorInterActiveThread extends Thread {
    private static final String TAG = "InteractiveService";

    private LocalServerSocket serverSocket;
    private LocalSocket socket;

    private DataOutputStream writeStream;
    private DataInputStream readStream;
    private final Object lock_data = new Object();

    private final MainActivity activity;

    private final ArrayList<InterActiveEntity> activeList = new ArrayList<>();

    public EmulatorInterActiveThread(MainActivity activity) {
        this.activity = activity;
    }

    @Override
    public void run() {
        super.run();
        initLink();
        Log.d(TAG, "initLink end");
        readData();
        sendData();
    }

    public void postData(int action, byte[] data, byte[] raw) {
        synchronized (lock_data) {
            InterActiveEntity entity = new InterActiveEntity(action, data, raw);
            activeList.add(entity);
//            Log.e(TAG, "postData: " + raw.length);
        }
    }

    private void sendData() {
        while (true) {
            try {
                if (writeStream == null) {
                    ThreadUtil.sleep(1);
                    continue;
                }
                InterActiveEntity next = null;
                synchronized (lock_data) {
                    if (activeList.size() > 0) {
                        next = activeList.get(0);
                        activeList.remove(0);
                    }
                }
                if (next == null) {
                    next = new InterActiveEntity(ActionConstant.ACTION_CHECK_LINK, new byte[0], new byte[0]);
                }
                writeStream.writeInt(next.action);
                writeStream.writeInt(next.data.length);
                writeStream.writeInt(next.raw.length);
                if (next.data.length != 0)
                    writeStream.write(next.data);
                if (next.raw.length != 0)
                    writeStream.write(next.raw);
                if (next.action == ActionConstant.ACTION_CHECK_LINK)
                    ThreadUtil.sleepMillis(100L);
                if (next.data.length != 0 || next.raw.length != 0) {
                    Log.e(TAG, "sendData: " + next.data.length + next.raw.length);
                }
            } catch (Throwable e) {
                Log.e(TAG, "sendData: ", e);
                closeSocket();
                initLink();
            }
        }
    }

    private void readData() {
        ThreadUtil.runOnNewThread(() -> {
            while (true) {
                try {
                    if (readStream == null) {
                        ThreadUtil.sleep(1);
                        continue;
                    }
                    int action = readStream.readInt();
                    int dataLen = readStream.readInt();
                    int rawLen = readStream.readInt();
                    byte[] data = new byte[dataLen];
                    if (dataLen != 0)
                        readStream.readFully(data);
                    byte[] raw = new byte[rawLen];
                    if (rawLen != 0)
                        readStream.readFully(raw);
                    activity.handleData(action, data, raw);
                } catch (Throwable e) {
                    Log.e(TAG, "readData error: ", e);
                    closeSocket();
                    initLink();
                }
            }
        });
    }

    private void initLink() {
        while (true) {
            try {
                if (serverSocket == null)
                    serverSocket = new LocalServerSocket(TransitPathConstant.UNIX_INTERACTIVE + "/rootfs");
                socket = serverSocket.accept();
                writeStream = new DataOutputStream(socket.getOutputStream());
                readStream = new DataInputStream(socket.getInputStream());
                break;
            } catch (IOException e) {
                Log.e(TAG, "createServiceSocket: ", e);
                closeSocket();
                ThreadUtil.sleep(1);
            }
        }
    }

    private void closeSocket() {
        SocketUtil.close(socket);
        SocketUtil.close(writeStream);
        SocketUtil.close(readStream);
        socket = null;
        writeStream = null;
        readStream = null;
    }
}
