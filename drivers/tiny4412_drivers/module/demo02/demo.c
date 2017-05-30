/**
 *插入到pc机的驱动模块
 *
 */
#include <linux/init.h>
#include <linux/module.h>

static int __init demo_init(void)
{
	printk("hello,word! driver module is inserted!\n");

	return 0;
}

module_init(demo_init);


static void __exit demo_exit(void)
{
	printk("goodbye, word! driver is removed!\n");
} 

module_exit(demo_exit);

MODULE_LICENSE("GPL");

MODULE_AUTHOR("Songze Lee");
MODULE_VERSION("version v1.0");
MODULE_DESCRIPTION("It is a simple demo for driver module");
