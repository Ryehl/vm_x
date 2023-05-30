package com.magic.vm.cpputil;

public class GpsFun {
    static {
        System.loadLibrary("gps");
    }
    public native static void startGps(String vm_id);

    public native static void updateGps(double latitude, double longitude,
                                        double metersElevation, int nSatellites);
}
