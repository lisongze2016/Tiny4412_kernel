/**
 * 驱动模块调用子函数
 * 注意Makefile的编写
 */

#include <linux/init.h>
#include <linux/module.h>

extern void call_func0(void);

static int __init demo_init(void)
{
	call_func0();
	printk("hello,word! driver module is inserted!\n");

	return 0;
}

module_init(demo_init);/*指定驱动模块入口点*/


static void __exit demo_exit(void)
{
	printk("goodbye, word! driver is removed!\n");
} 

module_exit(demo_exit);/*指定驱动模块出口*/

MODULE_LICENSE("GPL");/*公共许可声明*/

MODULE_AUTHOR("Songze Lee");/*作者*/
MODULE_VERSION("Verson 1.0");/*版本号*/
MODULE_DESCRIPTION("It is a simple demo for driver module");/*描述*/
