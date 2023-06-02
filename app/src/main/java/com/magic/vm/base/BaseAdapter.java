package com.magic.vm.base;

import android.content.Context;
import android.view.LayoutInflater;
import android.view.View;
import android.view.ViewGroup;

import androidx.annotation.IdRes;
import androidx.annotation.NonNull;
import androidx.recyclerview.widget.RecyclerView;

import java.util.ArrayList;

public abstract class BaseAdapter<D> extends RecyclerView.Adapter<BaseAdapter.ViewHolder> {
    protected ArrayList<D> data;

    @NonNull
    @Override
    public ViewHolder onCreateViewHolder(@NonNull ViewGroup parent, int viewType) {
        View inflate = LayoutInflater.from(parent.getContext()).inflate(getItemId(), parent, false);
        return new ViewHolder(inflate);
    }

    public ArrayList<D> getData() {
        return data;
    }

    protected abstract int getItemId();

    @Override
    public int getItemCount() {
        return data == null ? 0 : data.size();
    }

    public void setData(ArrayList<D> data) {
        this.data = data;
        notifyDataSetChanged();
    }

    public void addData(ArrayList<D> data) {
        if (data == null) return;
        if (this.data == null)
            this.data = data;
        this.data.addAll(data);
    }

    public static class ViewHolder extends RecyclerView.ViewHolder {

        public Context context;

        public ViewHolder(@NonNull View itemView) {
            super(itemView);
            context = itemView.getContext();
        }

        public <T extends View> T findViewById(@IdRes int id) {
            return itemView.findViewById(id);
        }
    }
}
