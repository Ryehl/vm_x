package com.magic.vm.boxutil;

import android.content.ClipData;
import android.content.ClipboardManager;
import android.content.Context;
import android.net.LocalServerSocket;
import android.net.LocalSocket;
import android.os.Build;
import android.text.TextUtils;
import android.util.Log;

import java.io.DataInputStream;
import java.io.DataOutputStream;
import java.io.IOException;
import java.nio.charset.StandardCharsets;

public class TransitClipboardThread extends Thread {
    private static final String TAG = "xxx_clip";
    private ClipboardManager clipboardManager;
    private LocalServerSocket localServerSocket;
    private LocalSocket localClientSocket;
    private DataInputStream inputStream;
    private DataOutputStream outputStream;

    private String lastClipText = null;
    private Context context;

    private static class ClipCode {
        private static final int CLIP_CONTENT = 0x01;
        private static final int CLIP_CLEAR = 0x02;
    }

    public void setContext(Context context) {
        this.context = context;
    }

    @Override
    public void run() {
        super.run();
        clipboardManager = (ClipboardManager) context.getSystemService(Context.CLIPBOARD_SERVICE);
        lastClipText = getClipText();
        initServiceSock();
        startStream();
    }

    private String getClipText() {
        ClipData primaryClip = clipboardManager.getPrimaryClip();
        if (primaryClip == null) return null;
        ClipData.Item itemAt = primaryClip.getItemAt(0);
        if (itemAt == null) return null;
        CharSequence text = itemAt.getText();
        return text.toString();
    }

    public void updateClipText() {
        String text = getClipText();
        if (text == null) {
            Log.e(TAG, "updateClipText: text = null");
            return;
        }
        if (TextUtils.equals(lastClipText, text)) {
            Log.e(TAG, "updateClipText: lastClipText = text");
            return;
        }
        if (outputStream == null) {
            Log.e(TAG, "updateClipText: outputStream = null");
            return;
        }
        try {
//            outputStream.writeInt(ClipCode.CLIP_CONTENT);
//            int length = text.length();
//            Log.e(TAG, "updateClipText: length = " + length);
//            outputStream.writeInt(length);
//            outputStream.write(text.getBytes(StandardCharsets.UTF_8));
            outputStream.writeUTF(text);
        } catch (Throwable e) {
            Log.e(TAG, "updateClipText error:", e);
        }
        lastClipText = text;
    }

    private void startStream() {
        while (true) {
            try {
                String s = inputStream.readUTF();
                setClipboardText(s);
                Log.e(TAG, "read: " + s);
            } catch (Throwable e) {
                Log.e(TAG, "loadStream error: ", e);
                closeStream();
                initServiceSock();
            }
        }
    }

    private void clearClip() {
        if (Build.VERSION.SDK_INT >= Build.VERSION_CODES.P) {
            clipboardManager.clearPrimaryClip();
            lastClipText = null;
        } else {
            setClipboardText("");
            lastClipText = "";
        }
    }

    private void closeStream() {
        SocketUtil.close(inputStream);
        SocketUtil.close(outputStream);
        SocketUtil.close(localClientSocket);
        inputStream = null;
        outputStream = null;
        localClientSocket = null;
    }

    private void initServiceSock() {
        while (true) {
            try {
                if (localServerSocket == null)
                    localServerSocket = new LocalServerSocket(TransitPathConstant.UNIX_CLIPBOARD + "/rootfs");
                localClientSocket = localServerSocket.accept();
                outputStream = new DataOutputStream(localClientSocket.getOutputStream());
                inputStream = new DataInputStream(localClientSocket.getInputStream());
                break;
            } catch (IOException e) {
                Log.e(TAG, "initServiceSock: " + e.getLocalizedMessage());
            }
        }
    }

    private void closeSock() {
        SocketUtil.close(inputStream);
        SocketUtil.close(outputStream);
        SocketUtil.close(localClientSocket);
        try {
            if (localServerSocket != null)
                localServerSocket.close();
        } catch (IOException ignore) {
        }
        inputStream = null;
        outputStream = null;
        localClientSocket = null;
        localServerSocket = null;
    }

    public void setClipboardText(String text) {
        clipboardManager.setPrimaryClip(ClipData.newPlainText(null, text));
        lastClipText = text;
    }
}
