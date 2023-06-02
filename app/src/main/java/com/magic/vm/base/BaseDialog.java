package com.magic.vm.base;

import android.app.Dialog;
import android.content.Context;
import android.view.ViewGroup;
import android.view.Window;
import android.view.WindowManager;

import androidx.annotation.NonNull;
import androidx.annotation.Nullable;

import com.cangyu.boxmagic.R;


public abstract class BaseDialog extends Dialog {
    protected int status = -1;

    public BaseDialog(@NonNull Context context) {
        this(context, R.style.baseDialog);
    }

    public BaseDialog(@NonNull Context context, int themeResId) {
        super(context, themeResId);
        setContentView(getLayoutId());
        getWindow(getWindow());
    }

    protected abstract int getLayoutId();


    protected void getWindow(@Nullable Window window) {
        if (window != null) {
            WindowManager.LayoutParams attr = window.getAttributes();
            if (attr != null) {
                attr.height = ViewGroup.LayoutParams.WRAP_CONTENT;
                attr.width = ViewGroup.LayoutParams.MATCH_PARENT;
                attr.gravity = getWindowGravity();
                window.setAttributes(attr);
            }
        }
    }

    protected abstract int getWindowGravity();
}
