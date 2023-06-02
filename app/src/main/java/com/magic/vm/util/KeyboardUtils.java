package com.magic.vm.util;

import android.content.Context;
import android.view.MotionEvent;
import android.view.View;
import android.view.inputmethod.InputMethodManager;
import android.widget.EditText;

import androidx.fragment.app.FragmentActivity;

public class KeyboardUtils {

    public static void hideKeyboard(FragmentActivity activity) {
        hideKeyboard(activity.getWindow().getDecorView());
    }

    /**
     * 隐藏输入法
     */
    public static void hideKeyboard(View view) {
        if (view == null || view.getContext() == null) {
            return;
        }
        Context context = view.getContext();
        InputMethodManager inputMethodManager = (InputMethodManager) context.getSystemService(Context.INPUT_METHOD_SERVICE);
        if (inputMethodManager != null && inputMethodManager.isActive()) {
            inputMethodManager.hideSoftInputFromWindow(view.getApplicationWindowToken(), 0);
        }
    }

    /**
     * 点击EditText外，则隐藏输入法
     */
    public static void hideKeyboardOutEditText(EditText view, MotionEvent event) {
        if (view == null || event == null) {
            return;
        }
        int[] location = new int[2];
        view.getLocationInWindow(location);
        int left = location[0];
        int top = location[1];
        int right = left + view.getWidth();
        int bottom = top + view.getHeight();
        float x = event.getX();
        float y = event.getY();
        if (x < left || x > right || top > y || bottom < y) {
            hideKeyboard(view);
        }
    }

    public static void showKeyboard(View view) {
        try {
            InputMethodManager imm = (InputMethodManager) view.getContext().getSystemService(Context.INPUT_METHOD_SERVICE);
            if (imm != null) {
                view.requestFocus();
                imm.showSoftInput(view, 0);
            }
        } catch (Exception e) {
            e.printStackTrace();
        }
    }
}
