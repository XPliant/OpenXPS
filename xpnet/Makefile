# Just the make file.
# Moved all module specific build instructions to Kbuild
#
#
# Copyright (c) 2015-2016 Cavium Inc. 

ifneq ($(KERNELRELEASE),)
# kbuild part of makefile
include Kbuild
else

KERNEL_SRC ?= /lib/modules/$(shell uname -r)/build

all:
	$(MAKE) -C $(KERNEL_SRC) M=$$PWD modules

modules_install:
	$(MAKE) -C $(KERNEL_SRC) M=$$PWD modules_install

clean:
	$(MAKE) -C $(KERNEL_SRC) M=$$PWD clean
	rm -rf *.o *.mod.* *.ko *.o.p
endif
