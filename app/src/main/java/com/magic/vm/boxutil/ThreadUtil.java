package com.magic.vm.boxutil;

import java.util.concurrent.ExecutorService;
import java.util.concurrent.Executors;

public class ThreadUtil {

    public static void sleep(int second) {
        try {
            Thread.sleep(second * 1000L);
        } catch (Throwable ignore) {
        }
    }
    
    public static void sleepMillis(long millis) {
        try {
            Thread.sleep(millis);
        } catch (Throwable ignore) {
        }
    }

    public static void runOnNewThread(Runnable runnable) {
        if (runnable == null) return;
        new Thread(runnable).start();
    }

    public static ExecutorService createThreadPool(int fixedPoolSize) {
        return Executors.newFixedThreadPool(fixedPoolSize);
    }
}
