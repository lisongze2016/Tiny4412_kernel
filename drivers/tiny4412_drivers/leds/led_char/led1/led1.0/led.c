/**
 *4LED 1亮 0灭
 *早期经典的字符设备注册方法
 *register_chrdev
 *write read
 *
 */

#include <linux/init.h>
#include <linux/module.h>
#include <linux/fs.h>
#include <linux/io.h>
#include <linux/uaccess.h>
#include <linux/slab.h>

#define LED_MAJOR 185

#define GPM4BASE 0x11000000
#define GPM4CON  0x2e0
#define GPM4DAT  0x2e4

static char *VGPM4BASE = NULL;

#define VGPM4CON *((u32 *)(VGPM4BASE+GPM4CON))
#define VGPM4DAT *((u32 *)(VGPM4BASE+GPM4DAT))


static void led_init(void)
{
	VGPM4CON &= ~0xffff;
	VGPM4CON |= 0x1111;/*设置输出引脚*/

	VGPM4DAT |= 0x0f;/*led all off*/
}


static void led_stat(char *buf)
{
	u32 tmp;
	int i;
	
	tmp = VGPM4DAT;
	
	for(i = 0; i < 4; i++){
	
		if(tmp & (1<<i)){
			buf[i] = 0;
		}else{
			buf[i] = 1;
		}
	}
}

static void led_ctrl(int num, int stat)
{
	
	if(stat){
		VGPM4DAT &= ~(1 << (num - 1));
	}else{
		VGPM4DAT |= (1 << (num - 1));
	}
}

static int
exynos4412_led_open(struct inode *inodp, struct file *filp)
{
	return 0;
}

static ssize_t 
exynos4412_led_read(struct file *filp, char __user *buf, size_t cnt, loff_t *fpos)
{
	char stat[4] = {0};
	
	led_stat(stat);	

	if(copy_to_user(buf,stat,sizeof(stat)))
	{
		return -EINVAL;
	}
	return sizeof(stat);
}

static ssize_t
exynos4412_led_write(struct file *filp, const char __user *buf, size_t cnt, loff_t *fpos)
{
	u8 cmd;
	u8 num,stat;
	if(copy_from_user(&cmd,buf,cnt)){//get_user(cmd,buf);
		return -EINVAL;
	}

	num = cmd >> 4;stat = cmd & 0xf;//cmd 高4位放led编号 低4位放led状态

	if(( num < 1 ) || ( num > 4 ) ){
	        return -EINVAL;
	}
	if(( stat != 0 ) && ( stat != 1 )){
	        return -EINVAL;
	}
	
	led_ctrl(num, stat);
	
	return 1;

}
static int 
exynos4412_led_release(struct inode *inodp, struct file *filp)
{
	return 0;
}


/**字符驱动的核心
 *当应用程序操作设备文件时所调用的open read write等函数
 *最终会调用这个结构中对应的函数
 */
static struct file_operations exynos4412_led_fops = {
	.owner   = THIS_MODULE,/*内核使用这个字段以避免在模块的操作正在被使用时卸载该模块*/
	.open    = exynos4412_led_open,
	.read    = exynos4412_led_read,
	.write   = exynos4412_led_write,
	.release = exynos4412_led_release/*当file结构被释放时，将调用这个操作*/
};

static int __init exynos4412_led_init(void)
{
	VGPM4BASE = ioremap(GPM4BASE,SZ_4K);
	if(NULL == VGPM4BASE){
		return -ENOMEM;
	}
	led_init();
	return register_chrdev(LED_MAJOR,"LED",&exynos4412_led_fops);
}
module_init(exynos4412_led_init);

static void __exit exynos4412_led_exit(void)
{
	unregister_chrdev(LED_MAJOR,"LED");
	printk("goodbye! led  driver\n");
}
module_exit(exynos4412_led_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Songze Lee");
MODULE_VERSION("verson 1.0");
MODULE_DESCRIPTION("char driver for led");

