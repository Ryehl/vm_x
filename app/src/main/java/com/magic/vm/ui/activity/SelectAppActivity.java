package com.magic.vm.ui.activity;

import android.content.Intent;
import android.content.pm.PackageInfo;
import android.os.Bundle;
import android.util.Log;
import android.widget.ImageView;
import android.widget.TextView;

import androidx.recyclerview.widget.GridLayoutManager;
import androidx.recyclerview.widget.RecyclerView;

import com.magic.vm.R;
import com.magic.vm.base.BaseActivity;
import com.magic.vm.entity.SelectAppBean;
import com.magic.vm.ui.adapter.SelectAppAdapter;
import com.magic.vm.util.AppUtil;

import java.util.ArrayList;

public class SelectAppActivity extends BaseActivity {
    private static final String TAG = "SelectAppActivity";
    private static final String PARAM_APP_LIST = "p_app_list";
    public static final String PARAM_INSTALLED_APP_LIST = "p_installed_list";

    private ImageView ivBack;
    private ImageView ivSearch;
    private ImageView ivRefresh;
    private TextView tvAdd;

    private SelectAppAdapter adapter;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_select_app);
        initView();
        initListener();
        loadAppList();
        resetBtnText();
    }

    private void resetBtnText() {
        ArrayList<SelectAppBean> data = adapter.getData();
        if (data == null || data.isEmpty()) {
            tvAdd.setAlpha(0.5f);
            tvAdd.setText(getString(R.string.add_apps_0));
            return;
        }
        int num = 0;
        for (SelectAppBean temp : data) {
            if (temp.isSelect && !temp.isInstalled) {
                num++;
            }
        }
        if (num == 0) {
            tvAdd.setAlpha(0.5f);
            tvAdd.setText(getString(R.string.add_apps_0));
            return;
        }
        tvAdd.setText(getString(R.string.add_apps, String.valueOf(num)));
        tvAdd.setAlpha(1);
    }

    private void loadAppList() {
        ArrayList<String> installedList = getIntent().getStringArrayListExtra(PARAM_INSTALLED_APP_LIST);
        if (installedList == null) installedList = new ArrayList<>();
        ArrayList<PackageInfo> installedApp = AppUtil.getInstalledApp(this);
        ArrayList<SelectAppBean> selectAppBeans = new ArrayList<>();
        for (PackageInfo packageInfo : installedApp) {
            SelectAppBean e = new SelectAppBean();
            e.info = packageInfo;
            selectAppBeans.add(e);
            e.isInstalled = installedList.contains(packageInfo.packageName);
        }
        Log.i(TAG, "loadAppList: appNum = " + selectAppBeans.size());
        adapter.setData(selectAppBeans);
    }

    private void initListener() {
        ivBack.setOnClickListener(v -> finish());
        tvAdd.setOnClickListener(v -> {
            Intent intent = getIntent();
            //将选中的列表返回给MainActivity
            ArrayList<PackageInfo> packageList = new ArrayList<>();
            ArrayList<SelectAppBean> data = adapter.getData();
            if (data != null) {
                for (SelectAppBean temp : data) {
                    if (temp.isSelect && !temp.isInstalled) {
                        packageList.add(temp.info);
                    }
                }
            }
            intent.putParcelableArrayListExtra(PARAM_APP_LIST, packageList);
            setResult(RESULT_OK, intent);
            finish();
        });
        ivSearch.setOnClickListener(v -> {
            Intent intent = new Intent(this, SearchAppActivity.class);
            startActivity(intent);
        });
        ivRefresh.setOnClickListener(v -> {
            adapter.setData(new ArrayList<>());
            loadAppList();
            resetBtnText();
        });

        adapter.setListener((position, bean) -> {
            if (!bean.isInstalled) {
                bean.isSelect = !bean.isSelect;
                adapter.notifyItemChanged(position);
                resetBtnText();
            }
        });
    }

    private void initView() {
        ivBack = findViewById(R.id.iv_back);
        ivSearch = findViewById(R.id.iv_search);
        ivRefresh = findViewById(R.id.iv_refresh);
        tvAdd = findViewById(R.id.tv_add);
        RecyclerView recyclerView = findViewById(R.id.recy_apps);
        adapter = new SelectAppAdapter();
        recyclerView.setAdapter(adapter);
        recyclerView.setLayoutManager(new GridLayoutManager(this, 4));
    }
}