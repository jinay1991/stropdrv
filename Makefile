CC=gcc
MODULE_NAME=LDD_CaseChange

KERNEL_SOURCE:=/lib/modules/$(shell uname -r)/build
obj-m+=chardrv_mutex.o
all:
	$(MAKE) -C $(KERNEL_SOURCE) M=$(shell pwd) modules
clean:
	$(MAKE) -C $(KERNEL_SOURCE) M=$(shell pwd) clean
