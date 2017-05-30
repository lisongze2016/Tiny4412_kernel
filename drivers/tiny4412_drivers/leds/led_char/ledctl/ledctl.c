#include <linux/init.h>
#include <linux/module.h>
#include <linux/fs.h>
#include <linux/uaccess.h>
#include <linux/slab.h>
#include <linux/io.h>
#include <linux/ioctl.h>

#include "ledioctl.h"

#define GPM4BASE 0x11000000
#define GPM4CON  0x2e0
#define GPM4DAT  0x2e4

static unsigned int major = 185; 

static char *vir = NULL;

static void led_ctrl(int num, int stat)
{
	u32 tmp;

	tmp = ioread8(vir + GPM4DAT);
	
	if(stat){
		tmp &= ~(1 << (num - 1));
	}else{
		tmp |= (1 << (num - 1));
	}

	iowrite8(tmp, vir + GPM4DAT);
}

static void led_init(void)
{
	u32 tmp;

	tmp = ioread32(vir + GPM4CON);
	tmp &= ~0xffff;
	tmp |= 0x1111;
	iowrite32(tmp, vir + GPM4CON);

	tmp = ioread8(vir + GPM4DAT);
	tmp |= 0xf;
	iowrite8(tmp, vir + GPM4DAT);
}

static void led_stat(char *buf)
{
	u32 tmp;
	int i;

	tmp = ioread8(vir + GPM4DAT);

	for(i = 0; i < 4; i++){
		if(tmp & (1 << i)){
			buf[i] = 0;
		}else{
			buf[i] = 1;
		}
	}
}

static int my_open (struct inode *inodp, struct file *filp)
{

	return 0;
}

static ssize_t 
my_read (struct file *filp, char __user *buf, size_t cnt, loff_t *fpos)
{
	char stat[4] = {0};

	led_stat(stat);

	if(copy_to_user(buf, stat, sizeof(stat))){
		return -EINVAL;
	}

	return sizeof(stat);
}


static ssize_t 
my_write (struct file *filp, const char __user *buf, size_t cnt, loff_t *fpos)
{
	u8 cmd;	
	u8 num, stat;

	get_user(cmd, buf);

	num = cmd >> 4; stat = cmd & 0xf;

	if(( num < 1 ) || ( num > 4 ) ){
		return -EINVAL;
	}

	if(( stat != 0 ) && ( stat != 1 )){
		return -EINVAL;
	}

	led_ctrl(num, stat);

	return 1;
}

static long 
my_ioctl (struct file *filp, unsigned int request, unsigned long arg)
{
	char stat[4] = {0};
	u8 cmd;	
	u8 num, usrcmd;

	
	if(_IOC_TYPE(request) == LEDTYPE){

		switch(_IOC_NR(request)){
			case 0: 
				led_stat(stat);
				if(copy_to_user((char *)arg, stat, sizeof(stat))){
					return -EINVAL;
				}
				break;
			case 1:
				get_user(cmd, (char *)arg);
				num = cmd >> 4; usrcmd = cmd & 0xf;
				if(( num < 1 ) || ( num > 4 ) ){
					return -EINVAL;
				}
				if(( usrcmd != 0 ) && ( usrcmd != 1 )){
					return -EINVAL;
				}
				led_ctrl(num, usrcmd);
				break;
			default:
				return -EINVAL;
		}
	}	

	return 0;
}

static int     
my_release (struct inode *inodp, struct file *filp)
{
	return 0;
}

static struct file_operations fops = {
	.owner = THIS_MODULE,
	.open = my_open,	
	.read = my_read,
	.write = my_write,
	.unlocked_ioctl = my_ioctl,
	.release = my_release,
};

static int __init demo_init(void)
{
	vir = ioremap(GPM4BASE, SZ_4K);

	if(NULL == vir){
		return -ENOMEM;
	}
	
	led_init();
	return	register_chrdev(major, "ledmy", &fops);
}

module_init(demo_init);


static void __exit demo_exit(void)
{
	led_init();
	unregister_chrdev(major, "myled");
	iounmap(vir);
} 

module_exit(demo_exit);

MODULE_LICENSE("GPL");

MODULE_AUTHOR("Songze Lee");
MODULE_VERSION("version v1.0");
MODULE_DESCRIPTION("It is a simple demo for driver module");
