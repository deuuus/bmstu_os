CURRENT = $(shell uname -r)
KDIR = /lib/modules/$(CURRENT)/build
PWD = $(shell pwd)
TARGET = fortune
obj-m := $(TARGET).o

default:
	$(MAKE) -C $(KDIR) M=$(PWD) modules

clean:
	@rm -f *.o .*.cmd .*.flags *.mod.c *.order Module.*
	@rm -f .*.*.cmd *~ *.*~ TODO.*
	@rm -fR .tmp*
	@rm -rf .tmp_versions
	@rm *.ko *.mod

disclean: clean
	@rm *.ko *.symvers *.mod

load:
	sudo insmod ${TARGET}.ko

remove:
	sudo rmmod ${TARGET}.ko

info:
	modinfo ${TARGET}.ko

logs:
	sudo dmesg | grep "+:"

data:
	echo "aaa" > /proc/${TARGET}_dir/${TARGET}
	echo "bbb" > /proc/${TARGET}_dir/${TARGET}
	echo "ccc" > /proc/${TARGET}_dir/${TARGET}

read:
	cat /proc/${TARGET}_dir/${TARGET}

update:
	make clean && make remove