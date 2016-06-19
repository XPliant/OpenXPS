#/************************************************************************
#* Copyright (C) 2016, Cavium, Inc.
#* All Rights Reserved.
#*
#* This program is free software; you can redistribute it and/or
#* modify it under the terms of the GNU General Public License
#* as published by the Free Software Foundation; version 2
#* of the License.
#* 
#* This program is distributed in the hope that it will be useful,
#* but WITHOUT ANY WARRANTY; without even the implied warranty of
#* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
#* GNU General Public License for more details.
#* 
#* A copy of the GNU General Public License, version 2 is available in the file 
#* named LICENSE-GPLv2.md either in this directory or its root. 
#* Alernatively to obtain a copy, write to the Free Software Foundation, Inc., 
#* 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
#*
#* File: Makefile 
#* 
#* Abstract: This file contains the makefile required to complile the kernel 
#* Module (driver) called xp80-Pcie-Endpoint.ko 
#************************************************************************/

ifneq ($(KERNELRELEASE),)
# kbuild part of makefile
include Kbuild
else

KERNEL_SRC ?= /lib/modules/$(shell uname -r)/build

all:
	$(MAKE) -C $(KERNEL_SRC) M=$(PWD) modules

modules_install:
	$(MAKE) -C $(KERNEL_SRC) M=$(PWD) modules_install

clean:
	$(MAKE) -C $(KERNEL_SRC) M=$(PWD) clean
	rm -rf *.o *.mod.* *.ko *.o.p
endif
