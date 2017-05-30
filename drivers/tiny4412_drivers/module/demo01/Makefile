
obj-m	:= demo.o

OUR_KERNEL := /home/lisongze/tiny4412/Tiny4412_kernel/

all:
	make -C $(OUR_KERNEL) M=$(shell pwd) modules
#	进入到变量目录(内核源码目录)下编译  M 编译pwd路径下的模块
clean:
	make -C $(OUR_KERNEL) M=`pwd` clean 

