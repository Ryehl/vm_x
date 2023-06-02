package com.magic.vm.snapdatathread;

import android.net.LocalServerSocket;
import android.net.LocalSocket;
import android.util.Log;

import com.magic.vm.boxutil.ThreadUtil;
import com.magic.vm.entity.TransitPathConstant;
import com.magic.vm.snapdatathread.TransitSensorThread;

import java.io.IOException;

public class SensorServiceThread extends Thread {
    private static final String TAG = "xxx sensor";

    private LocalServerSocket localServerSocket;

    @Override
    public void run() {
        super.run();
        initService();
    }

    private void initService() {
        while (true) {
            try {
                if (localServerSocket == null)
                    localServerSocket = new LocalServerSocket(TransitPathConstant.UNIX_SENSOR + "/rootfs");
                LocalSocket client = localServerSocket.accept();
                TransitSensorThread transitSensorThread = new TransitSensorThread();
                transitSensorThread.setClient(client);
                transitSensorThread.start();
            } catch (IOException e) {
                Log.e(TAG, "initService error: ", e);
                ThreadUtil.sleep(3);
            }
        }
    }

}
