package com.magic.vm.util;

import org.apache.commons.compress.archivers.tar.TarArchiveEntry;
import org.apache.commons.compress.archivers.tar.TarArchiveInputStream;
import org.apache.commons.compress.compressors.xz.XZCompressorInputStream;

import java.io.File;
import java.io.FileOutputStream;
import java.io.IOException;
import java.io.InputStream;

public class Extractor {

    public static boolean extract(InputStream inputStream, String extractDir) {
        try (TarArchiveInputStream tarArchiveInputStream = new TarArchiveInputStream(new XZCompressorInputStream(inputStream))) {
            byte[] buffer = new byte[4096];
            TarArchiveEntry entry;
            while ((entry = tarArchiveInputStream.getNextTarEntry()) != null) {
                String outputPath = extractDir + "/" + entry.getName();
                File outputFile = new File(outputPath);
                if (entry.isDirectory()) {
                    outputFile.mkdirs();
                } else if (entry.isSymbolicLink()) {
                    String linkName = entry.getLinkName();
                    if (linkName.startsWith("/")) {
                        linkName = extractDir + linkName;
                    }
                    FileUtils.symlink(linkName, outputPath);
                } else {
                    outputFile.createNewFile();
                    try (FileOutputStream fos = new FileOutputStream(outputFile)) {
                        int len;
                        while ((len = tarArchiveInputStream.read(buffer)) > 0) {
                            fos.write(buffer, 0, len);
                        }
                    }
                }
                FileUtils.chmod(outputPath, entry.getMode());
            }
        } catch (IOException e) {
            e.printStackTrace();
            return false;
        }
        return true;
    }
}
