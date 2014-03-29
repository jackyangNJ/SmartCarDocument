package spiLib;

public class SPIFunc {

    int devFd;
    int speed = 500000;

    public SPIFunc(String devName) {
        init(devName, speed);
    }

    void init(String devName, int speed) {
        this.speed = speed;
        devFd = SPILib.INSTANCE.SPISetup(devName, speed);
        if (devFd == -1) {
            System.err.println("cannot open file" + devName);
        }
    }

    public SPIFunc(String devName, int speed) {
        init(devName, speed);
    }

    public int RWBytes(byte[] data, int len) {
        return SPILib.INSTANCE.SPIDataRW(devFd, speed, data, len);
    }
    public int shutdown(int fd){
        return SPILib.INSTANCE.shutdown(fd);
    }
}
