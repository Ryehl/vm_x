package com.magic.vm.boxutil;

public class ByteUtil {
    public static int byteArr2IntVal(byte[] source, int start) {
        int result = 0;
        for (int i = start; i < source.length && i < start + 4; i++)
            result |= ((0xFF & source[i]) << ((i - start) * 8));
        return result;
    }
}
