
obj-m	:= team_hehe.o
team_hehe-objs	:= demo.o fun0.o fun1.o

OUR_KERNEL := /home/lisongze/tiny4412/Tiny4412_kernel/

all:
	make -C $(OUR_KERNEL) M=$(shell pwd) modules
clean:
	make -C $(OUR_KERNEL) M=`pwd` clean 

