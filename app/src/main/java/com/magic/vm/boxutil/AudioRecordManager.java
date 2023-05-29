package com.magic.vm.boxutil;

import android.Manifest;
import android.content.pm.PackageManager;
import android.media.AudioFormat;
import android.media.AudioRecord;
import android.media.MediaRecorder;

import androidx.core.app.ActivityCompat;

import com.magic.vm.App;

public class AudioRecordManager {

    private static int getFormat(int nativeFormat) {
        switch (nativeFormat) {
            case 2:
                return AudioFormat.ENCODING_PCM_8BIT;
            case 5:
                return AudioFormat.ENCODING_PCM_FLOAT;
            case 1:
            default:
                return AudioFormat.ENCODING_PCM_16BIT;
        }
    }

    public static AudioRecord createAudioRecord(int sampleRateHz, int channelConfig, int audioFormat) {
        audioFormat = getFormat(audioFormat);
        int bufferSize = 2 * AudioRecord.getMinBufferSize(sampleRateHz, channelConfig, audioFormat);//缓冲区大小
        if (ActivityCompat.checkSelfPermission(App.getContext(), Manifest.permission.RECORD_AUDIO) != PackageManager.PERMISSION_GRANTED) {
            return null;
        }
        return new AudioRecord(MediaRecorder.AudioSource.MIC, sampleRateHz, channelConfig, audioFormat, bufferSize);
    }
}
