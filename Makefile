obj-m += drv.o
all:
	make -C /lib/modules/4.15.0-60-generic/build M=$(PWD) modules

clean:
	make -C /lib/modules/4.15.0-60-generic/build M=$(PWD) clean
