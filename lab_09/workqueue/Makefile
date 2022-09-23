ifneq ($(KERNELRELEASE),)
	obj-m   := my_wq.o
else
	CURRENT = $(shell uname -r)
	KDIR = /lib/modules/$(CURRENT)/build 
	PWD = $(shell pwd)

default: 
	$(MAKE) -C $(KDIR) M=$(PWD) modules 
	
clean:  
	@rm -f *.o .*.cmd .*.flags *.mod *.mod.c *.order 
	@rm -f .*.*.cmd *~ *.*~ TODO.* 
	@rm -fR .tmp* 
	@rm -rf .tmp_versions 

disclean: clean 
	@rm *.ko *.symvers 

load:
	sudo insmod my_wq.ko

remove:
	sudo rmmod my_wq.ko

lsmod:
	sudo lsmod | grep my_wq

log:
	sudo dmesg | grep "+"

cat:
	cat /proc/my_wq

intr:
	cat /proc/interrupts

endif
