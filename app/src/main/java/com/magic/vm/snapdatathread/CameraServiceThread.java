package com.magic.vm.snapdatathread;

import android.content.Context;
import android.net.LocalServerSocket;
import android.net.LocalSocket;
import android.util.Log;

import com.magic.vm.boxutil.ThreadUtil;
import com.magic.vm.entity.TransitPathConstant;
import com.magic.vm.snapdatathread.TransitCameraThread;

import java.io.IOException;

public class CameraServiceThread extends Thread {
    private static final String TAG = "xxxCamera";
    //camera
    private final Context mContext;
    private final String vm_id;

    //socket
    private LocalServerSocket localServerSocket;

    public CameraServiceThread(Context context, String vm_id) {
        this.mContext = context;
        this.vm_id = vm_id;
        if (mContext == null || vm_id == null) {
            throw new RuntimeException("CameraServiceThread context = null | vm_id = null");
        }
    }

    @Override
    public void run() {
        while (true) {
            try {
                if (localServerSocket == null)
                    localServerSocket = new LocalServerSocket(TransitPathConstant.UNIX_CAMERA + vm_id);
                Log.e(TAG, "connectSock: start");
                LocalSocket accept = localServerSocket.accept();
                new TransitCameraThread(mContext, accept).start();
                Log.e(TAG, "connectSock: accept");
            } catch (IOException e) {
                Log.e(TAG, "connectSock: ", e);
                ThreadUtil.sleep(1);
            }
        }
    }
}
