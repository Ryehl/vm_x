package com.magic.vm.boxutil;

import android.content.Context;
import android.hardware.Sensor;
import android.hardware.SensorEvent;
import android.hardware.SensorEventListener;
import android.hardware.SensorManager;
import android.net.LocalServerSocket;
import android.net.LocalSocket;
import android.util.Log;

import com.magic.vm.App;

import java.io.DataInputStream;
import java.io.DataOutputStream;
import java.io.IOException;

public class SensorServiceThread extends Thread  {
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
