package com.magic.vm.ui.activity;

import android.content.pm.PackageInfo;
import android.os.Bundle;
import android.text.Editable;
import android.util.Log;
import android.view.inputmethod.EditorInfo;
import android.widget.EditText;
import android.widget.ImageView;
import android.widget.TextView;

import com.magic.vm.R;
import com.magic.vm.base.BaseActivity;
import com.magic.vm.entity.SelectAppBean;
import com.magic.vm.interfaces.AfterTextChange;
import com.magic.vm.util.AppUtil;
import com.magic.vm.util.KeyboardUtils;

import java.util.ArrayList;

public class SearchAppActivity extends BaseActivity {

    private EditText etSearch;

    private ImageView ivNoData;
    private TextView tvNoData;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_search_app);
        initView();
        loadAppList();
        resetBtnText();
    }

    private void resetBtnText() {
    }

    private void loadAppList() {
        ArrayList<String> installedList = getIntent().getStringArrayListExtra(SelectAppActivity.PARAM_INSTALLED_APP_LIST);
        if (installedList == null) installedList = new ArrayList<>();
        ArrayList<PackageInfo> installedApp = AppUtil.getInstalledApp(this);
        ArrayList<SelectAppBean> selectAppBeans = new ArrayList<>();
        for (PackageInfo packageInfo : installedApp) {
            SelectAppBean e = new SelectAppBean();
            e.info = packageInfo;
            selectAppBeans.add(e);
            e.isInstalled = installedList.contains(packageInfo.packageName);
        }
        Log.i("TAG", "loadAppList: appNum = " + selectAppBeans.size());
//        adapter.setData(selectAppBeans);
    }

    private void initView() {
        findViewById(R.id.iv_back).setOnClickListener(v -> finish());
        etSearch = findViewById(R.id.et_search);
        ivNoData = findViewById(R.id.iv_nodata);
        tvNoData = findViewById(R.id.tv_nodata);

        etSearch.setOnEditorActionListener((v, actionId, event) -> {
            if (actionId == EditorInfo.IME_ACTION_SEARCH) {
                KeyboardUtils.hideKeyboard(this);
                return true;
            }
            return false;
        });
        etSearch.addTextChangedListener(new AfterTextChange() {
            @Override
            public void afterTextChanged(Editable s) {
                Log.e("TAG", "afterTextChanged: " + s.toString() + " -- " + etSearch.getText().toString());
            }
        });
    }

    @Override
    protected boolean isImmersiveBar() {
        return true;
    }
}