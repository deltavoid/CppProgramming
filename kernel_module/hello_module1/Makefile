

# MODULE := hello
# obj-m += $(MODULE).o
# CFLAGS_$(MODULE).o := -DDEBUG
# KERNDIR	:= /lib/modules/$(shell uname -r)/build


# .PHONY: insmod rmmod build clean
# default: insmod

# build: $(MODULE).ko
# $(MODULE).ko: $(MODULE).c
# 	$(MAKE) -C $(KERNDIR) M=$(CURDIR) modules

# clean:
# 	rm -rf *.ko *.o  *.mod *.mod.o *.mod.c *.order *.symvers .$(MODULE).*.cmd .tmp_versions

# insmod: rmmod build
# 	sudo insmod $(MODULE).ko

# rmmod:
# 	-sudo rmmod $(MODULE)



MODULE_NAME		:= hello
# MODULE_LICENSE		:= GPL
# MODULE_VERSION		:= 1.0
# MODULE_AUTHOR		:= Muchun Song <songmuchun@bytedance.com>
# MODULE_DESCRIPTION	:= Kprobe template for easy register kernel probes

ifneq ($(KERNELRELEASE),)
obj-m			:= $(MODULE_NAME).o
# $(MODULE_NAME)-m	:= init.o
# $(MODULE_NAME)-m	+= kprobe.o trace.o
# ldflags-y		+= -r -T $(KBUILD_EXTMOD)/kprobe.lds
# ccflags-y		+= -I$(KBUILD_EXTMOD)/include

# name-fix   = $(squote)$(quote)$(subst $(comma),_,$(subst -,_,$1))$(quote)$(squote)
# ccflags-y += -DCONFIG_MODULE_NAME=$(call name-fix,$(MODULE_NAME))
# ccflags-y += -DCONFIG_MODULE_AUTHOR=$(call name-fix,$(MODULE_AUTHOR))
# ccflags-y += -DCONFIG_MODULE_VERSION=$(call name-fix,$(MODULE_VERSION))
# ccflags-y += -DCONFIG_MODULE_DESCRIPTION=$(call name-fix,$(MODULE_DESCRIPTION))
# ccflags-y += -DCONFIG_MODULE_LICENSE=$(call name-fix,$(MODULE_LICENSE))
else
PWD			:= $(shell pwd)
KERNEL_HEAD		:= $(shell uname -r)
KERNEL_DIR		:= /lib/modules/$(KERNEL_HEAD)/build

build:
	$(MAKE) -C $(KERNEL_DIR) M=$(PWD) modules

clean:
	$(MAKE) -C $(KERNEL_DIR) M=$(PWD) clean


insmod: rmmod build
	sudo insmod $(MODULE_NAME).ko

rmmod:
	-sudo rmmod $(MODULE_NAME)


endif