package com.magic.vm.util;

import android.content.Context;
import android.content.pm.ApplicationInfo;
import android.content.pm.PackageInfo;
import android.content.pm.PackageManager;
import android.graphics.drawable.Drawable;

import androidx.annotation.NonNull;

import java.util.ArrayList;
import java.util.List;

public class AppUtil {
    public static String getAppVer(Context context) {
        if (context == null) return null;
        PackageManager manager = context.getPackageManager();
        try {
            PackageInfo info = manager.getPackageInfo(context.getPackageName(), 0);
            return info.versionName;
        } catch (PackageManager.NameNotFoundException e) {
            return null;
        }
    }


    // 通过packName得到PackageInfo，作为参数传入即可
    private static boolean isSystemApp(@NonNull PackageInfo pi) {
        boolean isSysApp = (pi.applicationInfo.flags & ApplicationInfo.FLAG_SYSTEM) == 1;
        boolean isSysUpd = (pi.applicationInfo.flags & ApplicationInfo.FLAG_UPDATED_SYSTEM_APP) == 1 << 7;
        return isSysApp || isSysUpd;
    }

    @NonNull
    public static ArrayList<PackageInfo> getInstalledApp(Context context) {
        ArrayList<PackageInfo> packageInfoList = new ArrayList<>();
        if (context == null) return packageInfoList;
        List<PackageInfo> installedPackages = context.getPackageManager().getInstalledPackages(0);
        for (PackageInfo info : installedPackages) {
            if (!isSystemApp(info)) {
                packageInfoList.add(info);
            }
        }
        return packageInfoList;
    }

    public static String getAppName(Context context, PackageInfo info) {
        if (info == null || context == null) return null;
        return info.applicationInfo.loadLabel(context.getPackageManager()).toString();
    }

    public static Drawable getAppIcon(Context context, PackageInfo info) {
        if (info == null || context == null) return null;
        return info.applicationInfo.loadIcon(context.getPackageManager());
    }
}
