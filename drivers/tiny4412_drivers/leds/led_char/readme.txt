led1/led1.0/   
 早期经典的字符设备注册方法
 register_chrdev
 write read

led1/led1.1/  
 早期经典的字符设备注册方法
 register_chrdev
 write read
 自动创建设备节点class_create device_create

led2
register_chrdev_region
cdev_init cdev_add
write read

led3
misc 混杂设备
gpio

ledctl
ioctl

led_app.c
需要在arm-linux-gcc 编译在linux环境开发板上运行，
如需在Android开发板下运行，需要编写Android.mk文件,在android源码中编译，
生成可执行文件运行


