package com.magic.vm.util;

public class FileUtils {

    public native static int chmod(String path, int mode);

    public native static int symlink(String path1, String path2);
}
