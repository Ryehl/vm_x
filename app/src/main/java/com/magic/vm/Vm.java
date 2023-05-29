package com.magic.vm;

public class Vm {

    static {
        System.loadLibrary("vm");
    }

    public native static void start(String rootPath);
}
