

MODULE := kretprobe_example
obj-m += $(MODULE).o
CFLAGS_$(MODULE).o := -DDEBUG
KERNDIR	:= /lib/modules/$(shell uname -r)/build


.PHONY: insmod rmmod build clean
default: insmod

build: $(MODULE).ko
$(MODULE).ko: $(MODULE).c
	$(MAKE) -C $(KERNDIR) M=$(CURDIR) modules

clean:
	rm -f *.ko *.o  *.mod *.mod.o *.mod.c *.order *.symvers .$(MODULE).*.cmd

insmod: rmmod build
	sudo insmod $(MODULE).ko symbol=__do_softirq

rmmod:
	-sudo rmmod $(MODULE)
