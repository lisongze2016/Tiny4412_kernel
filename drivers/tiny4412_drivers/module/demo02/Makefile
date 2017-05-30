
obj-m	:= demo.o

# pc机内核源码路径：/lib/modules/`uname -r`/build/
OUR_KERNEL := /lib/modules/`uname -r`/build/ 

all:
	make -C $(OUR_KERNEL) M=$(shell pwd) modules
clean:
	make -C $(OUR_KERNEL) M=`pwd` clean 

