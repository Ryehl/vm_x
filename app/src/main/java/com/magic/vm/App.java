package com.magic.vm;

import android.app.Application;
import android.content.Context;

import me.jessyan.autosize.AutoSizeConfig;
import me.jessyan.autosize.unit.Subunits;

public class App extends Application {
    private static Context context;

    @Override
    public void onCreate() {
        super.onCreate();
        context = this;

        AutoSizeConfig.getInstance()
                .setUseDeviceSize(false)
                .setCustomFragment(true)
                .getUnitsManager()
                .setSupportDP(true)
                .setSupportSP(true)
                .setSupportSubunits(Subunits.MM);
    }

    public static Context getContext() {
        return context;
    }
}
