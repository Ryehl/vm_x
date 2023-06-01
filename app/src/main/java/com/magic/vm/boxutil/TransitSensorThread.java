package com.magic.vm.boxutil;

import android.content.Context;
import android.hardware.Sensor;
import android.hardware.SensorEvent;
import android.hardware.SensorEventListener;
import android.hardware.SensorManager;
import android.net.LocalSocket;
import android.text.TextUtils;
import android.util.Log;

import com.magic.vm.App;
import com.magic.vm.cpputil.CameraFun;

import java.io.DataInputStream;
import java.io.DataOutputStream;
import java.io.IOException;
import java.nio.charset.StandardCharsets;
import java.util.Locale;

public class TransitSensorThread extends Thread implements SensorEventListener {
    private static final String TAG = "xxx_sensor";
    private LocalSocket client;
    private DataOutputStream outputStream;
    private DataInputStream inputStream;

    private final byte[] fill_in = new byte[256];
    private long time;
    private int delay;

    public void setClient(LocalSocket client) {
        try {
            this.client = client;
            outputStream = new DataOutputStream(client.getOutputStream());
            inputStream = new DataInputStream(client.getInputStream());
        } catch (Throwable e) {
            Log.e(TAG, "setClient error: ", e);
        }
    }

    @Override
    public void run() {
        super.run();
        if (client != null
                && outputStream != null
                && inputStream != null)
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
                String s = new String(content, 0, content.length);
                Log.e(TAG, "startStream_send: " + s);
                if (s.startsWith("list-sensors")) {
                    int sensorMask = getSensorMask();
                    sendLH(sensorMask);
                } else if (s.startsWith("time:")) {
                    String substring = s.substring("time:".length());
                    time = Long.parseLong(substring);
                } else if (s.startsWith("set-delay:")) {
                    String substring = s.substring("set-delay:".length());
                    delay = Integer.parseInt(substring);
                } else if (s.startsWith("set:")) {
                    String substring = s.substring("set:".length());
                    String[] split = substring.split(":");
                    if (split.length == 2) {//11280693883-12593168
                        int enable = Integer.parseInt(split[1]);
                        registSensor(split[0], enable);
                    }
                }
            } catch (Throwable e) {
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
                result |= (1 << i);
            }
        }
        return result;
    }

    private void registSensor(String tag, int enable) {
        if (tag == null) {
            return;
        }
        int type = -1;
        if (TextUtils.equals(tag, "acceleration")) {
            type = Sensor.TYPE_ACCELEROMETER;
        } else if (TextUtils.equals(tag, "gyroscope")) {
            type = Sensor.TYPE_GYROSCOPE;
        } else if (TextUtils.equals(tag, "magnetic-field")) {
            type = Sensor.TYPE_MAGNETIC_FIELD;
        } else if (TextUtils.equals(tag, "orientation")) {
            type = Sensor.TYPE_ORIENTATION;
        } else if (TextUtils.equals(tag, "temperature")) {
            type = Sensor.TYPE_TEMPERATURE;
        } else if (TextUtils.equals(tag, "proximity")) {
            type = Sensor.TYPE_PROXIMITY;
        } else if (TextUtils.equals(tag, "light")) {
            type = Sensor.TYPE_LIGHT;
        } else if (TextUtils.equals(tag, "pressure")) {
            type = Sensor.TYPE_PRESSURE;
        } else if (TextUtils.equals(tag, "humidity")) {
            type = Sensor.TYPE_RELATIVE_HUMIDITY;
        } else if (TextUtils.equals(tag, "magnetic-field-uncalibrated")) {
            type = Sensor.TYPE_MAGNETIC_FIELD_UNCALIBRATED;
        }
        //type = Sensor.TYPE_GYROSCOPE_UNCALIBRATED;
        if (type == -1)
            return;
        SensorManager sensorManager = (SensorManager) App.getContext().getSystemService(Context.SENSOR_SERVICE);
        Sensor tempSensor = sensorManager.getDefaultSensor(type);
        if (tempSensor != null) {
            if (enable == 0) {
                sensorManager.unregisterListener(this, tempSensor);
            } else {
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
        //"guest-sync:<time>"
        //"sync:<time>"
        if (outputStream == null) {
            SensorManager sensorManager = (SensorManager) App.getContext().getSystemService(Context.SENSOR_SERVICE);
            sensorManager.unregisterListener(this, event.sensor);
            return;
        }
        String info = null;
        if (event.sensor.getType() == Sensor.TYPE_ACCELEROMETER) {
            //"acceleration:<x>:<y>:<z>"
            info = String.format(Locale.getDefault(), "acceleration:%g:%g:%g", event.values[0], event.values[1], event.values[2]);
        } else if (event.sensor.getType() == Sensor.TYPE_GYROSCOPE) {
            //"gyroscope:<x>:<y>:<z>"
            info = String.format(Locale.getDefault(), "gyroscope:%g:%g:%g", event.values[0], event.values[1], event.values[2]);
        } else if (event.sensor.getType() == Sensor.TYPE_MAGNETIC_FIELD) {
            //"magnetic:<x>:<y>:<z>"
            info = String.format(Locale.getDefault(), "magnetic:%g:%g:%g", event.values[0], event.values[1], event.values[2]);
        } else if (event.sensor.getType() == Sensor.TYPE_ORIENTATION) {
            //"orientation:<azimuth>:<pitch>:<roll>"
            info = String.format(Locale.getDefault(), "orientation:%g:%g:%g", event.values[0], event.values[1], event.values[2]);
        } else if (event.sensor.getType() == Sensor.TYPE_TEMPERATURE) {
            //"temperature:<celsius>"  "temperature:%g"
            info = String.format(Locale.getDefault(), "temperature:%g", event.values[0]);
        } else if (event.sensor.getType() == Sensor.TYPE_PROXIMITY) {
            //"proximity:<value>"
            info = String.format(Locale.getDefault(), "proximity:%g", event.values[0]);
        } else if (event.sensor.getType() == Sensor.TYPE_LIGHT) {
            //"light:<lux>"
            info = String.format(Locale.getDefault(), "light:%g", event.values[0]);
        } else if (event.sensor.getType() == Sensor.TYPE_PRESSURE) {
            //"pressure:<hpa>"
            info = String.format(Locale.getDefault(), "pressure:%g", event.values[0]);
        } else if (event.sensor.getType() == Sensor.TYPE_RELATIVE_HUMIDITY) {
            //"humidity:<percent>"
            info = String.format(Locale.getDefault(), "humidity:%g", event.values[0]);
        } else if (event.sensor.getType() == Sensor.TYPE_GYROSCOPE_UNCALIBRATED) {
            info = String.format(Locale.getDefault(), "gyroscope:%g:%g:%g", event.values[0], event.values[1], event.values[2]);
        } else if (event.sensor.getType() == Sensor.TYPE_MAGNETIC_FIELD_UNCALIBRATED) {
            //"magnetic-uncalibrated:%g:%g:%g"
            info = String.format(Locale.getDefault(), "magnetic-uncalibrated:%g:%g:%g", event.values[0], event.values[1], event.values[2]);
        }
        send256string(info);
        send256string(String.format(Locale.getDefault(), "guest-sync:%d", CameraFun.getCameraFrameTime()));
        send256string(String.format(Locale.getDefault(), "sync:%d", CameraFun.getCameraFrameTime() / 1000000L + delay * 1000L));
    }

    private void send256string(String data) {
        if (data == null) {
            return;
        }
        if (outputStream == null) {
            return;
        }
        byte[] bytes = data.getBytes(StandardCharsets.UTF_8);
        if (bytes == null) {
            Log.e(TAG, "send256string bytes null, str:" + data);
            return;
        }
        try {
            outputStream.write(bytes);
            outputStream.write(fill_in, 0, fill_in.length - bytes.length);
        } catch (IOException e) {
            Log.e(TAG, "send256string err: " + e);
            closeSocket();
        }
    }

    @Override
    public void onAccuracyChanged(Sensor sensor, int accuracy) {

    }
}
