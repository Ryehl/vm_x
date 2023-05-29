package com.magic.vm.util;

import android.content.Context;

import androidx.annotation.Nullable;

public class DensityUtils {

    /**
     * dp 转为 px
     */
    public static int dp2px(@Nullable Context context, float dpValue) {
        if (context == null) return (int) (dpValue * 2);
        final float scale = context.getResources().getDisplayMetrics().density;
        return (int) (dpValue * scale + 0.5f);
    }

    /**
     * sp 转为 px
     */
    public static int sp2px(@Nullable Context context, float spValue) {
        if (context == null) return (int) (spValue * 2);
        final float fontScale = context.getResources().getDisplayMetrics().scaledDensity;
        return (int) (spValue * fontScale + 0.5f);
    }

    /**
     * px 转为 dp
     */
    public static int px2dp(Context context, float pxValue) {
        final float scale = context.getResources().getDisplayMetrics().density;
        return (int) (pxValue / scale + 0.5f);
    }

    /**
     * 获取状态栏高度
     */
    public static int getStatusHeight(Context context) {
        int statusBarHeight;
        int resourceId = context.getResources().getIdentifier("status_bar_height", "dimen", "android");
        if (resourceId > 0) {
            statusBarHeight = context.getResources().getDimensionPixelSize(resourceId);
        } else {
            statusBarHeight = dp2px(context, 38);
        }
        return statusBarHeight;
    }

    // 将px值转换为sp值
    public static int px2sp(Context context, float pxValue) {
        final float fontScale = context.getResources().getDisplayMetrics().scaledDensity;
        return (int) (pxValue / fontScale + 0.5f);
    }

    public static int getScreenHeight(Context context) {
//        WindowManager wm = (WindowManager) context.getSystemService(Context.WINDOW_SERVICE);
//        return wm.getDefaultDisplay().getHeight();
        return context.getResources().getDisplayMetrics().heightPixels;
    }

    public static int getScreenWidth(Context context) {
        return context.getResources().getDisplayMetrics().widthPixels;
    }
}
