obj-m := hello.o
#KERNEL_DIR := /lib/modules/$(shell uname -r)/build
KERNEL_DIR := /home/jack/Desktop/linux-xlnx-master
PWD := $(shell pwd)
all:
	make -C $(KERNEL_DIR) SUBDIRS=$(PWD) modules ARCH=arm
clean:
	rm *.o *.ko
