package com.magic.vm.snapdatathread;

import android.media.AudioRecord;
import android.media.AudioTrack;
import android.net.LocalServerSocket;
import android.net.LocalSocket;
import android.util.Log;

import com.magic.vm.boxutil.AudioHwCodes;
import com.magic.vm.boxutil.AudioRecordManager;
import com.magic.vm.boxutil.AudioTrackManager;
import com.magic.vm.boxutil.SocketUtil;
import com.magic.vm.boxutil.ThreadUtil;
import com.magic.vm.entity.TransitPathConstant;

import java.io.DataInputStream;
import java.io.DataOutputStream;
import java.io.InputStream;
import java.util.ArrayList;

public class TransitAudioHwThread extends Thread {

    private static final String TAG = "audio_hw";
    private static final Object lock_play = new Object();
    private LocalServerSocket localSocket;
    private LocalSocket accept;
    private DataInputStream readAudioStream;
    private DataOutputStream writeAudioStream;
    private AudioTrack audioTrack;
    private AudioRecord audioRecord;
    private final ArrayList<byte[]> play_cache = new ArrayList<>();
    private boolean run;
    private boolean startPlay = false;

    @Override
    public void run() {
        run = true;
        connect();
    }

    private int byteArr2IntVal(byte[] source, int start) {
        int result = 0;
        for (int i = start; i < source.length && i < start + 4; i++) {
            result |= ((0xFF & source[i]) << ((i - start) * 8));
        }
        return result;
    }

    private void startPlay() {
        ThreadUtil.runOnNewThread(() -> {
            while (startPlay) {
                try {
                    byte[] arr = null;
                    synchronized (lock_play) {
                        if (play_cache.size() > 0) {
                            arr = play_cache.get(0);
                            play_cache.remove(0);
                        }
                    }
                    if (audioTrack == null) break;
                    if (arr != null)
//                        audioTrack.write(arr, 0, arr.length, AudioTrack.WRITE_NON_BLOCKING);
                        audioTrack.write(arr, 0, arr.length);
                } catch (Throwable e) {
                    if (audioTrack != null) audioTrack.release();
                    play_cache.clear();
                    audioTrack = null;
                    break;
                }
            }
        });
    }

    private void startListen() {
        while (run) {
            try {
                byte[] byte_arr = new byte[4];
                readAudioStream.readFully(byte_arr);
                int code = byteArr2IntVal(byte_arr, 0);
                if (code == 0) continue;
                readAudioStream.readFully(byte_arr);
                int len = byteArr2IntVal(byte_arr, 0);
                byte[] data = new byte[len];
                readAudioStream.readFully(data);
                switch (code) {
                    case AudioHwCodes.OUT_INIT: {
                        int sampleRate = byteArr2IntVal(data, 0);
                        int channelMask = byteArr2IntVal(data, 4);
                        int format = byteArr2IntVal(data, 4 * 2);
                        //sampleRate = 44100, channelMask = 3, format = 1
                        Log.e(TAG, "startListen: AudioHwCodes.OUT_INIT sampleRate = " + sampleRate
                                + ", channelMask = " + channelMask + ", format = " + format);
                        if (audioTrack != null) {
                            audioTrack.release();
                            play_cache.clear();
                            audioTrack = null;
                        }
                        audioTrack = AudioTrackManager.createAudioTrack(sampleRate, channelMask, format);
//                        audioTrack = AudioTrackManager.createAudioTrack(44100, 3, 1);
                        startPlay = true;
                        startPlay();
                    }
                    break;
                    case AudioHwCodes.OUT_DATA: {
                        synchronized (lock_play) {
                            play_cache.add(data);
                        }
                    }
                    break;
                    case AudioHwCodes.OUT_CLOSE: {
                        startPlay = false;
                        if (audioTrack != null) {
                            audioTrack.release();
                            play_cache.clear();
                        }
                        audioTrack = null;
                    }
                    break;

                    case AudioHwCodes.IN_INIT: {
                        if (audioRecord != null) {
                            audioRecord.release();
                            audioRecord = null;
                        }
                        int sampleRate = byteArr2IntVal(data, 0);
                        int channelMask = byteArr2IntVal(data, 4);
                        int format = byteArr2IntVal(data, 4 * 2);
                        Log.e(TAG, "startListen: AudioHwCodes.IN_INIT sampleRate = " + sampleRate
                                + ", channelMask = " + channelMask + ", format = " + format);
                        audioRecord = AudioRecordManager.createAudioRecord(sampleRate, channelMask, format);
                    }
                    break;
                    case AudioHwCodes.IN_DATA: {
                        int read_len = byteArr2IntVal(data, 0);
                        byte[] bytes = new byte[read_len];
                        if (audioRecord != null) {
                            if (audioRecord.getRecordingState() != AudioRecord.RECORDSTATE_RECORDING) {
                                audioRecord.startRecording();
                            }
                            audioRecord.read(bytes, 0, read_len);
                        }
                        writeAudioStream.write(bytes);
                    }
                    break;
                    case AudioHwCodes.IN_CLOSE: {
                        if (audioRecord != null) {
                            audioRecord.stop();
                            audioRecord.release();
                        }
                        audioRecord = null;
                    }
                    break;

                    case AudioHwCodes.CLOSE_SOCK: {
                        Log.e(TAG, "startListen: closeConn");
                        closeConn();
                        run = false;
                    }
                    break;
                }
            } catch (Throwable e) {
                closeConn();
                Log.e(TAG, "startListen ERROR: ", e);
                break;
            }
        }
    }

    private void closeConn() {
        SocketUtil.close(writeAudioStream);
        SocketUtil.close(readAudioStream);
        SocketUtil.close(accept);
        writeAudioStream = null;
        readAudioStream = null;
        accept = null;
    }

    private void connect() {
        while (true) {
            try {
                Log.e(TAG, "AudioTrack: startRawListener");
                if (localSocket == null)
                    localSocket = new LocalServerSocket(TransitPathConstant.UNIX_AUDIO_HW + TransitPathConstant.UNIX_SUFFIX);
                accept = localSocket.accept();
                Log.e(TAG, "AudioTrack: accept audio hw");
                InputStream inputStream = accept.getInputStream();
                readAudioStream = new DataInputStream(inputStream);
                writeAudioStream = new DataOutputStream(accept.getOutputStream());
                startListen();
            } catch (Throwable e) {
                Log.e(TAG, "AudioTrack: startRawListener: " + e.getLocalizedMessage());
                closeConn();
                ThreadUtil.sleep(3);
            }
        }
    }
}
