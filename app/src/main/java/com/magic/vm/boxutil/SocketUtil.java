package com.magic.vm.boxutil;

import java.io.Closeable;

public class SocketUtil {
    public static void close(Closeable closeable) {
        try {
            if (closeable != null)
                closeable.close();
        } catch (Throwable ignore) {
        }
    }
}
