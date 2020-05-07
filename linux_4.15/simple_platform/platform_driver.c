#include <linux/module.h>
#include <linux/fs.h>
#include <linux/cdev.h>
#include <linux/device.h>
#include <linux/platform_device.h>
#include <asm/io.h>
 
static int beep_probe(struct platform_device *pdev)
{
	int ret;	
	printk("driver probe ok!");
	return 0;
}
 
static int beep_remove(struct platform_device *pdev)
{

	printk("driver remove ok!");
	return 0;
}
 
 
static struct platform_driver beep_driver=
{
    .driver.name = "bigbang",
    .probe = beep_probe,
    .remove = beep_remove,
};
 
 
static int beep_init(void)
{
	printk("driver module_init");
	
	return platform_driver_register(&beep_driver);
}
 
static void beep_exit(void)
{
	printk("driver module_exit");
	platform_driver_unregister(&beep_driver);
	
	return;
}
 
 
MODULE_LICENSE("GPL");
module_init(beep_init);
module_exit(beep_exit);
