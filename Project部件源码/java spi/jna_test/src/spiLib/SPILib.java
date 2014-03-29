package spiLib;


import com.sun.jna.Library;
import com.sun.jna.Native;


public interface SPILib extends Library {
	public static final String JNA_LIBRARY_NAME = "spi";
	public static final SPILib INSTANCE = (SPILib)Native.loadLibrary(SPILib.JNA_LIBRARY_NAME, SPILib.class);
	
	int SPIDataRW(int fd, int speed, byte []data, int len);
	int SPISetup(String spiPath, int speed);
        int shutdown(int fd);
}
