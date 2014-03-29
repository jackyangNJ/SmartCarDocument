ifneq ($(KERNELRELEASE),)
obj-m := Ulwave.o
else
KERNEL_DIR := /home/jack/Desktop/linux-xlnx-old
PWD := $(shell pwd)
all:
	$(MAKE) -C $(KERNEL_DIR) SUBDIRS=$(PWD) modules ARCH=arm
clean:
	rm *.o *.ko *.mod.c *.order *.symvers
endif
