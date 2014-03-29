/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */
package spiLib;

import com.sun.jna.Library;
import com.sun.jna.Native;
import com.sun.jna.Platform;
import com.sun.jna.Pointer;

public interface CLibrary extends Library {
    public final static int O_RDONLY = 00;
    public final static int O_WRONLY = 01;
    public final static int O_RDWR = 02;
    public static CLibrary INSTANCE = (CLibrary) Native.loadLibrary(Platform.C_LIBRARY_NAME, CLibrary.class);

    int printf(String format, Object... args);

    int open(String path, int flag);

    int close(int fd);

    int read(int fd, byte[] buf, int count);
}
