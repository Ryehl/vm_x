package com.magic.vm;

import android.Manifest;
import android.opengl.GLES20;
import android.os.Bundle;
import android.util.Log;
import android.view.KeyEvent;
import android.view.Surface;
import android.view.SurfaceHolder;
import android.view.SurfaceView;
import android.view.ViewGroup;
import android.widget.FrameLayout;

import androidx.annotation.NonNull;
import androidx.appcompat.app.AppCompatActivity;
import androidx.core.app.ActivityCompat;

import com.magic.vm.boxutil.CameraServiceThread;
import com.magic.vm.boxutil.EventParcelableUtil;
import com.magic.vm.boxutil.ThreadUtil;
import com.magic.vm.boxutil.TransitAudioHwThread;
import com.magic.vm.boxutil.TransitCameraThread;
import com.magic.vm.boxutil.TransitInputThread;
import com.magic.vm.boxutil.TransitPathConstant;
import com.magic.vm.util.DensityUtils;
import com.magic.vm.util.Extractor;

import java.io.File;
import java.io.IOException;
import java.io.InputStream;

public class MainActivity extends AppCompatActivity {

    static {
        System.loadLibrary("vm");
        System.loadLibrary("emugl");
        System.loadLibrary("input");
    }

    private static final String TAG = "===MainAct";
    private FrameLayout flLayout;
    private TransitInputThread thread;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);
        //fid
        initView();

        EventParcelableUtil.setScreenWidth(DensityUtils.getScreenWidth(this));

        new TransitAudioHwThread().start();
        //触摸事件
        thread = new TransitInputThread();
        thread.start();

        ActivityCompat.requestPermissions(this, new String[]{
                Manifest.permission.RECORD_AUDIO,
                Manifest.permission.CAMERA
        }, 0);
        new CameraServiceThread(this, TransitPathConstant.UNIX_SUFFIX).start();

        flLayout.post(() -> {
            int width = flLayout.getWidth();
            int height = flLayout.getHeight();

            bm_initRenderLib();

            bm_startRenderThread(width, height);

            createSurface();

            ThreadUtil.runOnNewThread(MainActivity::bm_startTcp);
            //start emu
            startVM();
        });
        flLayout.setKeepScreenOn(true);
    }


    /// --------  NativeMethods  --------///
    //<editor-fold desc="NativeMethods">
    public static native void bm_surfaceRedraw(Surface surface);

    public static native void bm_surfaceCreated(Surface surface);

    public static native void bm_surfaceChanged(Surface surface, int format, int width, int height);

    public static native void bm_surfaceDestroyed(Surface surface);

    public static native void bm_startRenderThread(int width, int height);

    public static native void bm_startTcp();

    public static native void bm_initRenderLib();

    //</editor-fold>

    private void createSurface() {
        SurfaceView surfaceView = new SurfaceView(this);
        SurfaceHolder holder = surfaceView.getHolder();
        holder.addCallback(new SurfaceHolder.Callback2() {
            @Override
            public void surfaceRedrawNeeded(@NonNull SurfaceHolder holder) {
                Log.e(TAG, "testEGL14 surfaceRedrawNeeded: ");
                bm_surfaceRedraw(holder.getSurface());
            }

            @Override
            public void surfaceCreated(@NonNull SurfaceHolder holder) {
                Log.e(TAG, "testEGL14 surfaceCreated: ");
                GLES20.glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
                holder.getSurface().isValid();

                bm_surfaceCreated(holder.getSurface());
            }

            @Override
            public void surfaceChanged(@NonNull SurfaceHolder holder, int format, int width, int height) {
                Log.e(TAG, "testEGL14 surfaceChanged: ");
                bm_surfaceChanged(holder.getSurface(), format, width, height);
            }

            @Override
            public void surfaceDestroyed(@NonNull SurfaceHolder holder) {
                Log.e(TAG, "testEGL14 surfaceDestroyed: ");
                bm_surfaceDestroyed(holder.getSurface());
            }
        });
        flLayout.addView(surfaceView, new FrameLayout.LayoutParams(ViewGroup.LayoutParams.MATCH_PARENT, ViewGroup.LayoutParams.MATCH_PARENT));
    }

    private void initView() {
        flLayout = findViewById(R.id.fl_show);

        flLayout.setOnTouchListener((v, event) -> {
            thread.postEvent(event, v.getWidth(), v.getHeight());
            return true;
        });
    }

    private void startVM() {
        new Thread(() -> {
            String rootfs = getDir("data", MODE_PRIVATE).getAbsolutePath() + "/rootfs";
            File file = new File(rootfs);
            if (!file.exists()) file.mkdirs();
            try (InputStream inputStream = getAssets().open("rom.tar.xz")) {
                Extractor.extract(inputStream, rootfs);
            } catch (IOException e) {
                Log.e("xxx", "onCreate: ", e);
            }
            Vm.start(rootfs);
        }).start();
    }


    @Override
    public boolean onKeyUp(int keyCode, KeyEvent event) {
        if (keyCode == KeyEvent.KEYCODE_BACK
                || keyCode == KeyEvent.KEYCODE_VOLUME_DOWN
                || keyCode == KeyEvent.KEYCODE_VOLUME_UP) {
            thread.postEvent(event, false);
            return true;
        }
        return super.onKeyUp(keyCode, event);
    }

    @Override
    public boolean onKeyDown(int keyCode, KeyEvent event) {
        if (keyCode == KeyEvent.KEYCODE_BACK
                || keyCode == KeyEvent.KEYCODE_VOLUME_DOWN
                || keyCode == KeyEvent.KEYCODE_VOLUME_UP) {
            thread.postEvent(event, true);
            return true;
        }
        return super.onKeyDown(keyCode, event);
    }
}
