
#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/miscdevice.h>
#include <linux/device.h>
#include <linux/fs.h>
#include <linux/types.h>
#include <linux/moduleparam.h>
#include <linux/slab.h>
#include <linux/ioctl.h>
#include <linux/cdev.h>
#include <linux/delay.h>
 
#include <linux/gpio.h>
#include <mach/gpio.h>
#include <plat/gpio-cfg.h>
#include <linux/platform_device.h>

#include <linux/leds.h>

struct led_desc {
	int gpio;
	char *name;
};

struct led_desc led_gpios[2];

struct led_classdev_4412 {
	struct led_classdev cdev;
	int gpio;
};


static struct led_classdev_4412 *led_devs;
static void	 brightness_set_4412(struct led_classdev *led_cdev,
			  enum led_brightness brightness)
{

	struct led_classdev_4412 *dev = container_of(led_cdev,struct led_classdev_4412,cdev);
	led_cdev->brightness = brightness;

	if (brightness != LED_OFF){
		gpio_request(dev->gpio,led_cdev->name);
		gpio_direction_output(dev->gpio,0);
		//gpio_free(dev->gpio);
	}
	else{
		gpio_request(dev->gpio,led_cdev->name);
		gpio_direction_output(dev->gpio,1);
		//gpio_free(dev->gpio);
	}
}


static int leds_init(void)
{
	int i;
	int ret;
	
	/* 1. alloc led_classdev */
	led_devs = kzalloc(sizeof(struct led_classdev_4412) * ARRAY_SIZE(led_gpios), GFP_KERNEL);
	if (led_devs == NULL) {
		printk("No memory for device\n");
		return -ENOMEM;
	}
	
	for (i = 0; i < ARRAY_SIZE(led_gpios); i++)
	{

		gpio_request(led_gpios[i].gpio, led_gpios[i].name);
		gpio_direction_output(led_gpios[i].gpio,1);/* all led off */
		//gpio_free(led_gpios[i].gpio);

		/* 2. set */
		led_devs[i].cdev.max_brightness = LED_FULL;
		led_devs[i].cdev.brightness_set = brightness_set_4412;
		led_devs[i].cdev.flags = LED_CORE_SUSPENDRESUME;
		led_devs[i].cdev.brightness = LED_OFF;
		led_devs[i].cdev.name = led_gpios[i].name;
		//led_devs[i].cdev.default_trigger = "timer";
		led_devs[i].gpio = led_gpios[i].gpio;

		/* 3. led_classdev_register */
		ret = led_classdev_register(NULL, &led_devs[i].cdev);
		if (ret) {
			i--;
			while (i >= 0) {
				led_classdev_unregister(&led_devs[i].cdev);
				i--;
			}
			kfree(led_devs);
			return -EIO;
		}
	}

	return 0;
}

static void leds_exit(void)
{
	int i;
	for (i = 0; i < ARRAY_SIZE(led_gpios); i++)
	{
		led_classdev_unregister(&led_devs[i].cdev);
		gpio_free(led_devs[i].gpio);
	}
	kfree(led_devs);
}

static int tiny4412_gpio_led_probe(struct platform_device *pdev)
{
	struct resource *gpio_res;

	printk("%s is enter!\n",__func__);
	gpio_res = platform_get_resource(pdev, IORESOURCE_MEM, 0);

	led_gpios[0].gpio = gpio_res->start;
	led_gpios[0].name = "led1";
	led_gpios[1].gpio = gpio_res->start + 1;
	led_gpios[1].name = "led2";

	leds_init();

	return 0;

}


static int tiny4412_gpio_led_remove(struct platform_device *pdev)
{
	leds_exit();
	return 0;
}



static struct platform_driver tiny4412_gpio_led_driver = {
	.probe		= tiny4412_gpio_led_probe,
	.remove		= tiny4412_gpio_led_remove,
	.driver		= {
		.name	= "tiny4412-leds-gpio",
		.owner	= THIS_MODULE,
	},
};


static int tiny4412_leds_init(void)
{
	return platform_driver_register(&tiny4412_gpio_led_driver);
}

static void __exit tiny4412_leds_exit(void)
{
	platform_driver_unregister(&tiny4412_gpio_led_driver);
}



module_init(tiny4412_leds_init);
module_exit(tiny4412_leds_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Songze Lee<Songze_Lee@163.com>");

