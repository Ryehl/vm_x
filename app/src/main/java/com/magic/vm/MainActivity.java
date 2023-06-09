package com.magic.vm;

import android.Manifest;
import android.content.Context;
import android.content.Intent;
import android.content.pm.PackageInfo;
import android.content.pm.PackageManager;
import android.location.Location;
import android.location.LocationManager;
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
import androidx.annotation.Nullable;
import androidx.appcompat.app.AppCompatActivity;
import androidx.core.app.ActivityCompat;

import com.google.gson.Gson;
import com.magic.vm.boxutil.EventParcelableUtil;
import com.magic.vm.boxutil.ThreadUtil;
import com.magic.vm.cpputil.GpsFun;
import com.magic.vm.entity.ActionConstant;
import com.magic.vm.entity.ApkTransportBean;
import com.magic.vm.entity.EmulatorInstallListBean;
import com.magic.vm.entity.TransitPathConstant;
import com.magic.vm.snapdatathread.CameraServiceThread;
import com.magic.vm.snapdatathread.EmulatorInterActiveThread;
import com.magic.vm.snapdatathread.SensorServiceThread;
import com.magic.vm.snapdatathread.TransitAudioHwThread;
import com.magic.vm.snapdatathread.TransitClipboardThread;
import com.magic.vm.snapdatathread.TransitInputThread;
import com.magic.vm.ui.activity.SelectAppActivity;
import com.magic.vm.util.AppUtil;
import com.magic.vm.util.DensityUtils;
import com.magic.vm.util.Extractor;

import java.io.File;
import java.io.FileInputStream;
import java.io.IOException;
import java.io.InputStream;
import java.nio.charset.StandardCharsets;
import java.util.ArrayList;
import java.util.List;

public class MainActivity extends AppCompatActivity {

    static {
        System.loadLibrary("vm");
        System.loadLibrary("emugl");
        System.loadLibrary("input");
    }

    private static final String TAG = "===MainAct";
    private static final String PARAM_APP_LIST = "p_app_list";
    private static final int FLAG_SELECT_APP = 0x1000;
    private FrameLayout flLayout;
    private TransitInputThread thread;
    private TransitClipboardThread transitClipboardThread;
    private EmulatorInterActiveThread interActiveThread;

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

        ActivityCompat.requestPermissions(this, new String[]{Manifest.permission.RECORD_AUDIO, Manifest.permission.CAMERA, Manifest.permission.ACCESS_COARSE_LOCATION, Manifest.permission.ACCESS_FINE_LOCATION,}, 0);
        new CameraServiceThread(this, TransitPathConstant.UNIX_SUFFIX).start();
        new SensorServiceThread().start();

        transitClipboardThread = new TransitClipboardThread();
        transitClipboardThread.setContext(this);
        transitClipboardThread.start();
        interActiveThread = new EmulatorInterActiveThread(this);
        interActiveThread.start();

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
        //屏幕保持常量
        flLayout.setKeepScreenOn(true);
        ThreadUtil.runOnNewThread(() -> {
            waitPermission(Manifest.permission.ACCESS_FINE_LOCATION);
            waitPermission(Manifest.permission.ACCESS_COARSE_LOCATION);
            updateLocation();
        });
    }

    private void updateLocation() {
        if (ActivityCompat.checkSelfPermission(this, Manifest.permission.ACCESS_FINE_LOCATION) != PackageManager.PERMISSION_GRANTED && ActivityCompat.checkSelfPermission(this, Manifest.permission.ACCESS_COARSE_LOCATION) != PackageManager.PERMISSION_GRANTED) {
            //assert true
            return;
        }
        //has permission
        LocationManager locationManager = (LocationManager) getSystemService(Context.LOCATION_SERVICE);
        List<String> providers = locationManager.getProviders(true);
        String locationProvider;
        if (providers.contains(LocationManager.NETWORK_PROVIDER)) {
            //如果是网络定位
            locationProvider = LocationManager.NETWORK_PROVIDER;
        } else if (providers.contains(LocationManager.GPS_PROVIDER)) {
            //如果是GPS定位
            locationProvider = LocationManager.GPS_PROVIDER;
        } else {
            Log.e("TAG", "没有可用的位置提供器");
            return;
        }
        Location lastKnownLocation = locationManager.getLastKnownLocation(locationProvider);
        if (lastKnownLocation != null) {
            GpsFun.updateGps(lastKnownLocation.getLatitude(), lastKnownLocation.getLongitude(), lastKnownLocation.getAltitude(), 0);//nSatellites unused
        }

        runOnUiThread(() -> {
            locationManager.requestLocationUpdates(locationProvider, 30 * 1000, 0, location -> {
                GpsFun.updateGps(location.getLatitude(), location.getLongitude(), location.getAltitude(), 0);//nSatellites unused
            });
        });
        GpsFun.startGps("/rootfs");
    }

    private void waitPermission(String permission) {
        while (true) {
            if (ActivityCompat.checkSelfPermission(this, permission) != PackageManager.PERMISSION_GRANTED) {
                ThreadUtil.sleep(3);
                continue;
            }
            break;
        }
    }


    public void handleData(int action, byte[] data, byte[] raw) {
        if (action != ActionConstant.ACTION_CHECK_LINK) Log.e(TAG, "handleData: " + action);
        switch (action) {
            case ActionConstant.ACTION_CHECK_LINK:
                //do noting
                break;
            case ActionConstant.ACTION_SETTING:
                runOnUiThread(() -> {
//                    HomeLeftDialog homeLeftDialog = new HomeLeftDialog(this);
//                    homeLeftDialog.show();
                });
                break;

            case ActionConstant.ACTION_VIP:
                runOnUiThread(() -> {
//                    Intent intent = new Intent(this, GetVipActivity.class);
//                    startActivity(intent);
                });
                break;

            case ActionConstant.ACTION_SHARE:
                runOnUiThread(() -> {
//                    ShareDialog shareDialog = new ShareDialog(this);
//                    shareDialog.show();
                });
                break;

            case ActionConstant.ACTION_FUN_GUIDE:
//                WebActivity.start(this, Constant.URL_GUIDE);
                break;

            case ActionConstant.ACTION_FUN_CREATE_VM:
                runOnUiThread(() -> {
//                    Intent intent = new Intent(this, DoubleOpenActivity.class);
//                    startActivity(intent);
                });
                break;

            case ActionConstant.ACTION_FUN_PINP:
                //画中画
                break;

            case ActionConstant.ACTION_ADD_APK:
                runOnUiThread(() -> {
                    String s = new String(data);
                    EmulatorInstallListBean bean = new Gson().fromJson(s, EmulatorInstallListBean.class);
                    Intent intent = new Intent(this, SelectAppActivity.class);
                    if (bean != null) {
                        intent.putStringArrayListExtra(SelectAppActivity.PARAM_INSTALLED_APP_LIST, new ArrayList<>(bean.getData()));
                    }
                    startActivityForResult(intent, FLAG_SELECT_APP);
                });
                break;
        }
    }

    @Override
    protected void onResume() {
        super.onResume();
        ThreadUtil.runOnNewThread(() -> {
            ThreadUtil.sleepMillis(100);
            if (transitClipboardThread != null) transitClipboardThread.updateClipText();
        });
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
    protected void onActivityResult(int requestCode, int resultCode, @Nullable Intent data) {
        super.onActivityResult(requestCode, resultCode, data);
        if (requestCode == FLAG_SELECT_APP
                && resultCode == RESULT_OK
                && data != null) {
            ArrayList<PackageInfo> appList = data.getParcelableArrayListExtra(PARAM_APP_LIST);
            if (appList == null || appList.size() == 0) return;
            ThreadUtil.runOnNewThread(() -> {
                for (PackageInfo packageInfo : appList) {
                    String appName = AppUtil.getAppName(this, packageInfo);
                    File file = new File(packageInfo.applicationInfo.sourceDir);
                    long length = file.length();
                    String info = new Gson().toJson(ApkTransportBean.create(length, appName, packageInfo.packageName));
                    try {
                        FileInputStream fileInputStream = new FileInputStream(file);
                        byte[] bytes = new byte[102400];
                        int len;

                        while ((len = fileInputStream.read(bytes)) != -1) {
                            //todo copy to vm
                        }
                    } catch (Throwable e) {
                        Log.e(TAG, "onActivityResult: ", e);
                    }
                }
            });

        }
    }


    @Override
    public boolean onKeyUp(int keyCode, KeyEvent event) {
        if (keyCode == KeyEvent.KEYCODE_BACK || keyCode == KeyEvent.KEYCODE_VOLUME_DOWN || keyCode == KeyEvent.KEYCODE_VOLUME_UP) {
            thread.postEvent(event, false);
            return true;
        }
        return super.onKeyUp(keyCode, event);
    }

    @Override
    public boolean onKeyDown(int keyCode, KeyEvent event) {
        if (keyCode == KeyEvent.KEYCODE_BACK || keyCode == KeyEvent.KEYCODE_VOLUME_DOWN || keyCode == KeyEvent.KEYCODE_VOLUME_UP) {
            thread.postEvent(event, true);
            return true;
        }
        return super.onKeyDown(keyCode, event);
    }
}
