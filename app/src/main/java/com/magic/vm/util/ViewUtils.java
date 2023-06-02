package com.magic.vm.util;

import android.app.Activity;
import android.graphics.Outline;
import android.view.HapticFeedbackConstants;
import android.view.View;
import android.view.ViewGroup;
import android.view.ViewOutlineProvider;
import android.view.Window;
import android.view.WindowManager;

import androidx.annotation.ColorInt;
import androidx.annotation.Nullable;

public class ViewUtils {

    /**
     * 填充沉浸式 并设置底色
     *
     * @param view     填充View
     * @param colorInt color
     */
    public static void initBarFillingView(@Nullable View view, @ColorInt int colorInt) {
        if (view == null) return;
        view.setVisibility(View.VISIBLE);
        ViewGroup.LayoutParams layoutParams = view.getLayoutParams();
        layoutParams.height = DensityUtils.getStatusHeight(view.getContext());
        view.setLayoutParams(layoutParams);
        view.setBackgroundColor(colorInt);
    }

    /**
     * View设置圆角
     */
    public static void outlineRadius(View view, final float radius) {
        view.setOutlineProvider(new ViewOutlineProvider() {
            @Override
            public void getOutline(View view, Outline outline) {
                outline.setRoundRect(0, 0, view.getWidth(), view.getHeight(), radius);
            }
        });
        view.setClipToOutline(true);
    }

    /**
     * 震动
     */
    public static void feedback(View view) {
        view.performHapticFeedback(HapticFeedbackConstants.LONG_PRESS);
    }

    /**
     * 沉浸式状态栏
     *
     * @param use   true=启用 false=清除
     * @param state true=黑字，白色主题
     *              false=白字，黑色主题
     */
    public static void immersiveBar(@Nullable Activity activity, boolean use, boolean state) {
        if (activity == null) return;
        Window window = activity.getWindow();
        if (window == null) return;
        if (use)
            window.addFlags(WindowManager.LayoutParams.FLAG_TRANSLUCENT_STATUS);
        else
            window.clearFlags(WindowManager.LayoutParams.FLAG_TRANSLUCENT_STATUS);

        //设置颜色
        setStatusBarColor(activity, state);
    }

    /**
     * 仅用于设置颜色
     *
     * @param state true=「黑字，白色主题」
     *              false=「白字，黑色主题」
     */
    public static void setStatusBarColor(@Nullable Activity activity, boolean state) {
        if (activity == null) return;
        Window window = activity.getWindow();
        if (window == null) return;
        View decorView = window.getDecorView();
        int originVisibility = decorView.getSystemUiVisibility();
        // 亮色模式，使用黑色文字
        if (state && (originVisibility & View.SYSTEM_UI_FLAG_LIGHT_STATUS_BAR) == 0) {
            originVisibility = originVisibility | View.SYSTEM_UI_FLAG_LIGHT_STATUS_BAR;
        }

        // 暗色模式，使用白色文字
        if (!state && (originVisibility & View.SYSTEM_UI_FLAG_LIGHT_STATUS_BAR) != 0) {
            originVisibility = originVisibility ^ View.SYSTEM_UI_FLAG_LIGHT_STATUS_BAR;
        }

        decorView.setSystemUiVisibility(originVisibility);
    }
}
