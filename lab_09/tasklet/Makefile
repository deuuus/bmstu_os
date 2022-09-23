ifneq ($(KERNELRELEASE),)
	obj-m   := tasklet.o
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
	sudo insmod tasklet.ko

remove:
	sudo rmmod tasklet.ko

lsmod:
	sudo lsmod | grep tasklet

log:
	sudo dmesg | grep "+"

endif
