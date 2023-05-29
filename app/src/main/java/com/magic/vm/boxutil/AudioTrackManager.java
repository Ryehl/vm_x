package com.magic.vm.boxutil;

import android.media.AudioFormat;
import android.media.AudioManager;
import android.media.AudioTrack;

import java.nio.ByteBuffer;
import java.nio.ByteOrder;
import java.nio.FloatBuffer;
import java.nio.ShortBuffer;
import java.util.HashMap;

public class AudioTrackManager {
    // private static final String TAG = "---audio manager";
    private final HashMap<String, AudioTrack> map;
    private static AudioTrackManager _INSTANCE;

    private AudioTrackManager() {
        map = new HashMap<>();
    }

    public static AudioTrackManager getInstance() {
        if (_INSTANCE == null) {
            synchronized (AudioTrackManager.class) {
                if (_INSTANCE == null) {
                    _INSTANCE = new AudioTrackManager();
                }
                return _INSTANCE;
            }
        }
        return _INSTANCE;
    }

    public AudioTrack getAudioTrack(int sampleRateInHz, boolean isDoublePipe, int audioFormat) {
        String key = "x_" + sampleRateInHz + (isDoublePipe ? "_t_" : "_f_") + audioFormat;
        AudioTrack track;
        synchronized (AudioTrackManager.class) {
            track = map.get(key);
            if (track == null) {
                track = createAudioTrack(sampleRateInHz, isDoublePipe, audioFormat);
                map.put(key, track);
            }
        }
        return track;
    }

    /**
     * 这些数据必须是原始数据 注意channel << 2
     *
     * @param audioRaw       pcm 原始数据
     * @param sampleRateInHz hz eg:8000, 16000, 22050, 24000, 32000, 44100, 47250, 48000
     * @param channel        AudioFormat.CHANNEL_OUT_STEREO  AudioFormat.CHANNEL_OUT_MONO
     * @param format         5,1 需要单独适配
     */
    public void playAudio(byte[] audioRaw, int sampleRateInHz, int channel, int format) {
        channel <<= 2;

        AudioTrack audioTrack = getAudioTrack(sampleRateInHz,
                channel == AudioFormat.CHANNEL_OUT_STEREO, format);

        if (checkData(audioRaw)) {
            if (channel == AudioFormat.CHANNEL_OUT_STEREO && format == 5) {
                float[] floats = byteArrayToFloatArray(audioRaw);
                audioTrack.write(floats, 0, floats.length, AudioTrack.WRITE_BLOCKING);
            } else {
                audioTrack.write(audioRaw, 0, audioRaw.length);
            }
        }
    }

    public boolean checkData(byte[] data) {
        boolean canUse = false;
        for (int i = 0; i < 100 && i < data.length; i++) {
            if (data[i] != 0) {
                canUse = true;
                break;
            }
        }
        return canUse;
    }

    public static float[] byteArrayToFloatArray(byte[] bytes) {
        ByteBuffer buffer = ByteBuffer.wrap(bytes);
        buffer.order(ByteOrder.LITTLE_ENDIAN);

        FloatBuffer fb = buffer.asFloatBuffer();
        float[] floatArray = new float[fb.limit()];
        fb.get(floatArray);
        return floatArray;
    }

    public static short[] byteArray2shortArray(byte[] bytes) {
        ByteBuffer buffer = ByteBuffer.wrap(bytes);
        buffer.order(ByteOrder.LITTLE_ENDIAN);

        ShortBuffer sb = buffer.asShortBuffer();
        short[] shorts = new short[sb.limit()];
        sb.get(shorts);
        return shorts;
    }

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

    public static AudioTrack createAudioTrack(int sampleRateInHz, int channelConfig, int audioFormat) {
        audioFormat = getFormat(audioFormat);
        channelConfig <<= 2;
        int bufSize = AudioTrack.getMinBufferSize(
                sampleRateInHz,
                channelConfig,
                audioFormat
        );
        // 实例化AudioTrack(设置缓冲区为最小缓冲区的2倍，至少要等于最小缓冲区)
        AudioTrack audioTrack = new AudioTrack(
                AudioManager.STREAM_MUSIC,
                sampleRateInHz,
                channelConfig,
                audioFormat,
                bufSize * 2,
                AudioTrack.MODE_STREAM
        );
        // 设置音量
        audioTrack.setVolume(AudioTrack.getMaxVolume());
        audioTrack.play();
        return audioTrack;
    }

    /**
     * @param sampleRateInHz 8000 , 16000 , 22050 , 24000 ,32000 , 44100 , 48000
     * @param isDoublePipe   是否双通道
     * @param audioFormat    AudioFormat.ENCODING_PCM_FLOAT
     */
    public static AudioTrack createAudioTrack(int sampleRateInHz, boolean isDoublePipe, int audioFormat) {
        // 获取最小缓冲区
        int channelConfig = isDoublePipe ? AudioFormat.CHANNEL_OUT_STEREO : AudioFormat.CHANNEL_OUT_MONO;
        audioFormat = getFormat(audioFormat);
        int bufSize = AudioTrack.getMinBufferSize(
                sampleRateInHz,
                channelConfig,
                audioFormat
        );
        // 实例化AudioTrack(设置缓冲区为最小缓冲区的2倍，至少要等于最小缓冲区)
        AudioTrack audioTrack = new AudioTrack(
                AudioManager.STREAM_MUSIC,
                sampleRateInHz,
                channelConfig,
                audioFormat,
                bufSize * 2,
                AudioTrack.MODE_STREAM
        );
        // 设置音量
        audioTrack.setVolume(AudioTrack.getMaxVolume());
        audioTrack.play();
        return audioTrack;
    }
}
