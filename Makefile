obj-m += drv.o
all:
	make -C /lib/modules/$(shell uname -r) M=$(PWD) modules

clean:
	make -C /lib/modules/$(shell uname -r) M=$(PWD) clean
