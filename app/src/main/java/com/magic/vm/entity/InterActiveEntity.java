package com.magic.vm.entity;

public class InterActiveEntity {
    public int action;
    public byte[] data;
    public byte[] raw;

    public InterActiveEntity() {
    }

    public InterActiveEntity(int action, byte[] data, byte[] raw) {
        this.action = action;
        this.data = data;
        this.raw = raw;
    }
}
