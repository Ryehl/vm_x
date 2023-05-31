package com.magic.vm.boxutil;

import android.content.Context;
import android.hardware.Sensor;
import android.hardware.SensorEvent;
import android.hardware.SensorEventListener;
import android.hardware.SensorManager;
import android.net.LocalSocket;
import android.util.Log;

import com.magic.vm.App;

import java.io.DataInputStream;
import java.io.DataOutputStream;
import java.io.IOException;

public class TransitSensorThread extends Thread implements SensorEventListener {
    private static final String TAG = "xxx sensor";
    private LocalSocket client;
    private DataOutputStream outputStream;
    private DataInputStream inputStream;

    public void setClient(LocalSocket client) {
        try {
            this.client = client;
            outputStream = new DataOutputStream(client.getOutputStream());
            inputStream = new DataInputStream(client.getInputStream());
        } catch (IOException e) {
            Log.e(TAG, "setClient error: ", e);
        }
    }

    @Override
    public void run() {
        super.run();
        if (client != null
                && outputStream == null
                && inputStream == null)
            startStream();
    }

    private void startStream() {
        while (true) {
            try {
                byte[] len_bytes = new byte[4];
                inputStream.readFully(len_bytes);
                int len = ByteUtil.byteArr2IntVal(len_bytes, 0);
                byte[] content = new byte[len];
                inputStream.readFully(content);
                String s = new String(content, 0, content.length - 1);
                Log.e(TAG, "startStream: " + s);

//                switch (code) {
//                    case SensorCode.SENSOR_LIST: {
//                        int sensorMask = getSensorMask();
//                        sendLH(sensorMask);
//                    }
//                    break;
//
//                    case SensorCode.SENSOR_WAKE: {
//                        //"acceleration:<x>:<y>:<z>"
//                        //"gyroscope:<x>:<y>:<z>"
//                        //"orientation:<azimuth>:<pitch>:<roll>"
//                        //"magnetic:<x>:<y>:<z>"
//                        //"magnetic-uncalibrated:%g:%g:%g"
//                        //"temperature:<celsius>"  "temperature:%g"
//                        //"proximity:<value>"
//                        //"light:<lux>"
//                        //"pressure:<hpa>"
//                        //"humidity:<percent>"
//                        //"guest-sync:<time>"
//                        //"sync:<time>"
//                    }
//                    break;
//                    case SensorCode.SENSOR_SET_DELAY: {
//                        //int ms ((int64_t)ns / 1000000)
//                    }
//                    break;
//                    case SensorCode.SENSOR_SET: {
//                        //name:enable
//                    }
//                    break;
//                    case SensorCode.SENSOR_TIME: {
//                        //longtime
//                    }
//                    break;
//                }
            } catch (IOException e) {
                Log.e(TAG, "startStream error: ", e);
                break;
            }
        }
        closeSocket();
    }

    private void closeSocket() {
        SocketUtil.close(inputStream);
        SocketUtil.close(outputStream);
        SocketUtil.close(client);
        inputStream = null;
        outputStream = null;
        client = null;
    }

    private int getSensorMask() {
        SensorManager sensorManager = (SensorManager) App.getContext().getSystemService(Context.SENSOR_SERVICE);
        int[] sensor_lists = {
                Sensor.TYPE_ACCELEROMETER,
                Sensor.TYPE_GYROSCOPE,
                Sensor.TYPE_MAGNETIC_FIELD,
                Sensor.TYPE_ORIENTATION,
                Sensor.TYPE_TEMPERATURE,
                Sensor.TYPE_PROXIMITY,
                Sensor.TYPE_LIGHT,
                Sensor.TYPE_PRESSURE,
                Sensor.TYPE_RELATIVE_HUMIDITY,
                Sensor.TYPE_GYROSCOPE_UNCALIBRATED,
                Sensor.TYPE_MAGNETIC_FIELD_UNCALIBRATED,
        };
        int result = 0;
        for (int i = 0; i < sensor_lists.length; i++) {
            Sensor tempSensor = sensorManager.getDefaultSensor(sensor_lists[i]);
            if (tempSensor != null) {
                result |= (i << i);
            }
        }
        return result;
    }

    private void initSensors() {
        SensorManager sensorManager = (SensorManager) App.getContext().getSystemService(Context.SENSOR_SERVICE);
        int[] sensor_lists = {
                Sensor.TYPE_ACCELEROMETER,
                Sensor.TYPE_GYROSCOPE,
                Sensor.TYPE_MAGNETIC_FIELD,
                Sensor.TYPE_ORIENTATION,
                Sensor.TYPE_TEMPERATURE,
                Sensor.TYPE_PROXIMITY,
                Sensor.TYPE_LIGHT,
                Sensor.TYPE_PRESSURE,
                Sensor.TYPE_RELATIVE_HUMIDITY,
                Sensor.TYPE_GYROSCOPE_UNCALIBRATED,
                Sensor.TYPE_MAGNETIC_FIELD_UNCALIBRATED,
        };
        for (int type : sensor_lists) {
            Sensor tempSensor = sensorManager.getDefaultSensor(type);
            if (tempSensor != null) {
                sensorManager.registerListener(this, tempSensor, SensorManager.SENSOR_DELAY_NORMAL);
            }
        }
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

    @Override
    public void onSensorChanged(SensorEvent event) {
        int type = event.sensor.getType();
        String name = event.sensor.getName();

    }

    @Override
    public void onAccuracyChanged(Sensor sensor, int accuracy) {

    }

    private static class SensorCode {
        private static final int SENSOR_LIST = 0x12;//list-sensors
        private static final int SENSOR_WAKE = 0x04;//wake
        private static final int SENSOR_SET_DELAY = 0x10;//set-delay:
        private static final int SENSOR_SET = 0x03;//set:
        private static final int SENSOR_TIME = 0x05;//time:
    }
}
