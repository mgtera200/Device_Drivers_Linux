obj-m += teraGPIO.o teraLED_RED.o teraLED_RED_2.o
teraGPIO-y := platform_driver.o file_operations.o
teraLED_RED-y := platform_device.o
teraLED_RED_2-y := platform_device2.o

all:
	make -C /lib/modules/$(shell uname -r)/build M=$(shell pwd) modules

clean:
	make -C /lib/modules/$(shell uname -r)/build M=$(shell pwd) clean