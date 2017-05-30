#include <linux/init.h>
#include <linux/module.h>

extern int sure;
extern void pri_value(int);

extern void call_func0(void);

static int __init demo_init(void)
{
	sure = 3856;

	pri_value(sure);
	
	call_func0();

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
