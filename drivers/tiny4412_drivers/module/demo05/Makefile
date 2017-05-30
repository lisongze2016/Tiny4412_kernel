
obj-m	:= test.o demo02.o
test-y 	:= demo01.o

OUR_KERNEL := /home/lisongze/tiny4412/Tiny4412_kernel/

all:
	make -C $(OUR_KERNEL) M=$(shell pwd) modules
install:
	make -C $(OUR_KERNEL) M=$(shell pwd) modules_install
clean:
	make -C $(OUR_KERNEL) M=`pwd` clean 

