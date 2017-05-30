/**
 *module_param(name, type, perm) 内核模块传参
 *向当前模块传参
 */


#include <linux/init.h>
#include <linux/module.h>

static int x_rel = 480, y_rel = 272;

module_param(x_rel, int, 0);
module_param(y_rel, int, 0);

static char *info = "mdg: lol?";

static int num=10;
module_param(num,int,S_IRUGO);
module_param(info, charp, 0);

static int __init demo_init(void)
{
	printk("hello,word! driver module is inserted!\n");

	printk("x: %d, y: %d\n %s\n", x_rel, y_rel, info);
	printk("num=%d\n",num);
	return 0;
}

static void __exit demo_exit(void)
{
	printk("goodbye, word! driver is removed!\n");
} 

module_init(demo_init);
module_exit(demo_exit);
MODULE_LICENSE("GPL");
MODULE_AUTHOR("Songze Lee");
MODULE_VERSION("version 1.0");
MODULE_DESCRIPTION("It is a simple demo for driver module");
