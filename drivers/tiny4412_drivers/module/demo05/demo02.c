#include <linux/init.h>
#include <linux/module.h>

static int sure = 9527;

/*符号导出 static定义的变量或函数其他模块可以调用*/
EXPORT_SYMBOL_GPL(sure);

static void pri_value(int val)
{
	printk("In %s: sure = %d\n", __FILE__, sure);
}

EXPORT_SYMBOL_GPL(pri_value);

static int __init demo_init(void)
{
	pri_value(sure);

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
