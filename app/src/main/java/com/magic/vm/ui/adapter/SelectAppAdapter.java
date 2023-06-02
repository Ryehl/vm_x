package com.magic.vm.ui.adapter;

import android.content.pm.PackageInfo;
import android.graphics.drawable.Drawable;
import android.view.View;
import android.widget.ImageView;
import android.widget.TextView;

import androidx.annotation.NonNull;

import com.magic.vm.R;
import com.magic.vm.base.BaseAdapter;
import com.magic.vm.entity.SelectAppBean;
import com.magic.vm.interfaces.IAdapterItemClickListener;
import com.magic.vm.util.AppUtil;
import com.magic.vm.util.DensityUtils;
import com.magic.vm.util.ViewUtils;

public class SelectAppAdapter extends BaseAdapter<SelectAppBean> {
    private IAdapterItemClickListener<SelectAppBean> listener;

    public void setListener(IAdapterItemClickListener<SelectAppBean> listener) {
        this.listener = listener;
    }

    @Override
    protected int getItemId() {
        return R.layout.item_select_app;
    }

    @Override
    public void onBindViewHolder(@NonNull BaseAdapter.ViewHolder holder, int position) {
        ImageView ivIcon = holder.findViewById(R.id.iv_appIcon);
        TextView tvLabel = holder.findViewById(R.id.tv_name);
        ImageView ivSelect = holder.findViewById(R.id.iv_select);
        View shadow = holder.findViewById(R.id.view_shadow);
        SelectAppBean selectAppBean = getData().get(position);
        PackageInfo info = selectAppBean.info;

        int dp_8 = DensityUtils.dp2px(holder.context, 8);
        ViewUtils.outlineRadius(ivIcon, dp_8);
        ViewUtils.outlineRadius(shadow, dp_8);

        String appName = AppUtil.getAppName(holder.context, info);
        if (appName != null)
            tvLabel.setText(appName);
        Drawable appIcon = AppUtil.getAppIcon(holder.context, info);
        if (appIcon != null)
            ivIcon.setImageDrawable(appIcon);

        if (selectAppBean.isInstalled) {
            shadow.setVisibility(View.VISIBLE);
            ivSelect.setVisibility(View.GONE);
        } else {
            shadow.setVisibility(View.GONE);
            ivSelect.setVisibility(View.VISIBLE);
            if (selectAppBean.isSelect) {
                ivSelect.setImageResource(R.mipmap.ic_select_s);
            } else {
                ivSelect.setImageResource(R.mipmap.ic_select_n);
            }
        }

        holder.itemView.setOnClickListener(v -> {
            if (listener != null) {
                listener.onItemClick(position, selectAppBean);
            }
        });
    }
}
