#include <linux/init.h>
#include <linux/module.h>

extern void call_func1(void);

void call_func0(void)
{
	call_func1();
}
