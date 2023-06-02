package com.magic.vm.entity;

import androidx.annotation.LayoutRes;
import androidx.annotation.StringRes;

public class TitleBean {
    @StringRes
    private int title;
    @LayoutRes
    private int rightLayoutRes;
    @LayoutRes
    private int leftLayoutRes;

    public TitleBean(int title) {
        this.title = title;
    }

    public TitleBean(int title, int rightLayoutRes) {
        this.title = title;
        this.rightLayoutRes = rightLayoutRes;
    }

    public TitleBean(int title, int rightLayoutRes, int leftLayoutRes) {
        this.title = title;
        this.rightLayoutRes = rightLayoutRes;
        this.leftLayoutRes = leftLayoutRes;
    }

    public int getTitle() {
        return title;
    }

    public void setTitle(int title) {
        this.title = title;
    }

    public int getRightLayoutRes() {
        return rightLayoutRes;
    }

    public void setRightLayoutRes(int rightLayoutRes) {
        this.rightLayoutRes = rightLayoutRes;
    }

    public int getLeftLayoutRes() {
        return leftLayoutRes;
    }

    public void setLeftLayoutRes(int leftLayoutRes) {
        this.leftLayoutRes = leftLayoutRes;
    }
}
