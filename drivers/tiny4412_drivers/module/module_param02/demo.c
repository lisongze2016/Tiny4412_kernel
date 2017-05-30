#include <linux/init.h>
#include <linux/module.h>

#define CNT 16

static int num = CNT;

static int array[CNT] = {1,2,3,4,5,6,7};

module_param_array(array, int, &num, 0);

static int __init demo_init(void)
{
	int i;

	printk("Insert module ok!\n");

	for(i = 0; i < num; i++){
		printk("array[%d] = %d\n", i, array[i]);
	}

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
