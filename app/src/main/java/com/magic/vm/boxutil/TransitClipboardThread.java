package com.magic.vm.boxutil;

import android.content.ClipData;
import android.content.ClipboardManager;
import android.content.Context;
import android.net.LocalServerSocket;
import android.net.LocalSocket;
import android.os.Build;
import android.text.TextUtils;
import android.util.Log;

import com.magic.vm.App;

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

    private static class ClipCode {
        private static final int CLIP_CONTENT = 0x01;
        private static final int CLIP_CLEAR = 0x02;
    }

    @Override
    public void run() {
        super.run();
        clipboardManager = (ClipboardManager) App.getContext().getSystemService(Context.CLIPBOARD_SERVICE);
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
        if (text == null)
            return;
        if (TextUtils.equals(lastClipText, text)) {
            return;
        }
        if (outputStream == null)
            return;
        try {
            outputStream.writeInt(ClipCode.CLIP_CONTENT);
            outputStream.writeInt(text.length());
            outputStream.write(text.getBytes(StandardCharsets.UTF_8));
        }catch (Throwable e) {
            Log.e(TAG, "updateClipText error:", e);
        }
        lastClipText = text;
    }

    private void startStream() {
        while (true) {
            try {
                int code = inputStream.readInt();
                int len = inputStream.readInt();
                byte[] content = new byte[len];
                inputStream.readFully(content);
                switch (code) {
                    case ClipCode.CLIP_CONTENT: {
                        String s = new String(content);
                        setClipboardText(s);
                    }
                    break;
                    case ClipCode.CLIP_CLEAR: {
                        clearClip();
                    }
                    break;
                }
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
