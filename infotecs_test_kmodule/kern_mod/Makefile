obj-m += module510.o
all: 
	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) modules
clean:
	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) clean
load: all
	sudo insmod module510.ko name=`cat ../name` per=`cat "../timer"`
unload:
	sudo rmmod module510.ko
test:
	sudo dmesg -C
	sudo insmod module510.ko name=`cat ../name` per=`cat "../timer"`
	sudo rmmod module510.ko
	sudo dmesg

