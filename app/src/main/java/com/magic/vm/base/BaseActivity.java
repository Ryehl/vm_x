package com.magic.vm.base;

import android.content.res.Configuration;
import android.content.res.Resources;
import android.os.Build;
import android.os.Bundle;
import android.view.LayoutInflater;
import android.view.MotionEvent;
import android.view.View;
import android.view.ViewGroup;
import android.widget.EditText;
import android.widget.ImageView;
import android.widget.LinearLayout;
import android.widget.RelativeLayout;
import android.widget.TextView;

import androidx.annotation.ColorInt;
import androidx.annotation.Nullable;
import androidx.annotation.StringRes;
import androidx.appcompat.app.AppCompatActivity;

import com.magic.vm.R;
import com.magic.vm.entity.TitleBean;
import com.magic.vm.util.KeyboardUtils;
import com.magic.vm.util.ViewUtils;

public abstract class BaseActivity extends AppCompatActivity {

    private Configuration configuration;

    @Override
    protected void onCreate(@Nullable Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        initStatusBar();
        ViewUtils.immersiveBar(this, isImmersiveBar(), isWhiteTheme());
    }

    @Override
    protected void onStart() {
        super.onStart();
        if (isImmersiveBar()) {
            ViewUtils.initBarFillingView(findViewById(R.id.tv_bar), getFillingBgColor());
        }
    }

    @ColorInt
    protected int getFillingBgColor() {
        return 0x00000000;
    }

    protected boolean isImmersiveBar() {
        return false;
    }

    protected boolean isWhiteTheme() {
        return true;
    }

    private void initStatusBar() {
        if (Build.VERSION.SDK_INT < Build.VERSION_CODES.M) {
            getWindow().setStatusBarColor(0XFF858585);
        } else {
            getWindow().setStatusBarColor(0XFFFFFFFF);
            getWindow().getDecorView().setSystemUiVisibility(View.SYSTEM_UI_FLAG_LIGHT_STATUS_BAR);
        }
    }

    @Override
    public void setTitle(@StringRes int titleId) {
        setTitle(getString(titleId));
    }

    @Override
    public void setTitle(CharSequence title) {
        TextView tvTitle = findViewById(R.id.tv_title);
        if (tvTitle != null) {
            tvTitle.setText(title);
        }
    }

    @Override
    public void setContentView(int layoutResID) {
        ViewGroup decorView = (ViewGroup) getWindow().getDecorView();
        View view = LayoutInflater.from(this).inflate(layoutResID, decorView, false);
        setContentView(view);
    }

    @Override
    public void setContentView(View view) {
        TitleBean titleBean = initTitle();
        if (titleBean == null) {
            super.setContentView(view);
            return;
        }
        ViewGroup decorView = (ViewGroup) getWindow().getDecorView();
        LinearLayout layoutTitle = (LinearLayout) LayoutInflater.from(this).inflate(R.layout.layout_toolbar, decorView, false);
        ImageView ivBack = layoutTitle.findViewById(R.id.iv_back);
        TextView tvTitle = layoutTitle.findViewById(R.id.tv_title);
        RelativeLayout rlToolbar = layoutTitle.findViewById(R.id.rl_toolbar);
        ivBack.setOnClickListener(v -> finish());
        if (titleBean.getTitle() != 0) {
            tvTitle.setText(getString(titleBean.getTitle()));
        }
        // 设置右侧的布局
        if (titleBean.getRightLayoutRes() != 0) {
            View rightLayout = LayoutInflater.from(this).inflate(titleBean.getRightLayoutRes(), decorView, false);
            RelativeLayout.LayoutParams params = new RelativeLayout.LayoutParams(ViewGroup.LayoutParams.WRAP_CONTENT, ViewGroup.LayoutParams.MATCH_PARENT);
            params.addRule(RelativeLayout.ALIGN_PARENT_END);
            rlToolbar.addView(rightLayout, params);
        }
        // 设置左侧的布局
        if (titleBean.getLeftLayoutRes() != 0) {
            View leftLayout = LayoutInflater.from(this).inflate(titleBean.getLeftLayoutRes(), decorView, false);
            RelativeLayout.LayoutParams params = new RelativeLayout.LayoutParams(ViewGroup.LayoutParams.WRAP_CONTENT, ViewGroup.LayoutParams.MATCH_PARENT);
            params.addRule(RelativeLayout.END_OF, R.id.iv_back);
            rlToolbar.addView(leftLayout, params);
        }
        LinearLayout contentView = new LinearLayout(this);
        contentView.setOrientation(LinearLayout.VERTICAL);
        contentView.addView(layoutTitle);
        contentView.addView(view);
        super.setContentView(contentView);
    }

    protected TitleBean initTitle() {
        return null;
    }

    @Override
    public boolean dispatchTouchEvent(MotionEvent ev) {
        View currentFocus = getCurrentFocus();
        if (ev != null && currentFocus != null && ev.getAction() == MotionEvent.ACTION_DOWN) {
            if (currentFocus instanceof EditText) {
                EditText editText = (EditText) currentFocus;
                KeyboardUtils.hideKeyboardOutEditText(editText, ev);
            }
        }
        return super.dispatchTouchEvent(ev);
    }

    /**
     * 字体大小不随着系统字体变化而变化，Activity在清单文件中需要配置
     * android:configChanges="fontScale"，否则字体变化时Activity 会重建
     */
    @Override
    public Resources getResources() {
        Resources resources = super.getResources();
        if (resources.getConfiguration().fontScale != 1F) {
            if (configuration == null) {
                configuration = new Configuration();
                configuration.setToDefaults();
            }
            resources.updateConfiguration(configuration, resources.getDisplayMetrics());
        }
        return resources;
    }
}
