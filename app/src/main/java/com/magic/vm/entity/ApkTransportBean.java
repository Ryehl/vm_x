package com.magic.vm.entity;

public class ApkTransportBean {
    private long fileSize;
    private String name;
    private String packageName;

    public static ApkTransportBean create(long fileSize, String name, String packageName) {
        return new ApkTransportBean(fileSize, name, packageName);
    }

    public ApkTransportBean() {
    }

    public ApkTransportBean(long fileSize, String name, String packageName) {
        this.fileSize = fileSize;
        this.name = name;
        this.packageName = packageName;
    }

    public long getFileSize() {
        return fileSize;
    }

    public void setFileSize(long fileSize) {
        this.fileSize = fileSize;
    }

    public String getName() {
        return name;
    }

    public void setName(String name) {
        this.name = name;
    }

    public String getPackageName() {
        return packageName;
    }

    public void setPackageName(String packageName) {
        this.packageName = packageName;
    }
}
