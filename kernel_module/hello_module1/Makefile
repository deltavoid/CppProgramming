

# MODULE := hello
# obj-m += $(MODULE).o
# CFLAGS_$(MODULE).o := -DDEBUG
# KERNDIR	:= /lib/modules/$(shell uname -r)/build




MODULE_NAME		:= hello


# ifneq ($(KERNELRELEASE),)
ifneq ($(KBUILD_EXTMOD),)
obj-m			:= $(MODULE_NAME).o
# $(MODULE_NAME)-m	:= init.o
# $(MODULE_NAME)-m	+= kprobe.o trace.o
# ldflags-y		+= -r -T $(KBUILD_EXTMOD)/kprobe.lds
# ccflags-y		+= -I$(KBUILD_EXTMOD)/include
ccflags-y		+= -DDEBUG

# name-fix   = $(squote)$(quote)$(subst $(comma),_,$(subst -,_,$1))$(quote)$(squote)
# ccflags-y += -DCONFIG_MODULE_NAME=$(call name-fix,$(MODULE_NAME))

KBUILD_MODPOST_WARN=1

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