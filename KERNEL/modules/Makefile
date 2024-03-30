obj-m += test.o

PWD := $(CURDIR)



all:

	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) modules
	# sudo insmod test.ko
	# dmesg | tail -n 10


clean:

	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) clean









