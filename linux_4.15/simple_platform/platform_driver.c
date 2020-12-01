#include <linux/module.h>
#include <linux/fs.h>
#include <linux/cdev.h>
#include <linux/device.h>
#include <linux/platform_device.h>
#include <linux/mod_devicetable.h>
#include <asm/io.h>
 
static int beep_probe(struct platform_device *pdev)
{
	int ret;	
	struct resource	*mem;
    mem = platform_get_resource(pdev, IORESOURCE_MEM, 0);
    
	printk(KERN_ERR "platform_driver_probe driver probe ok!\n");
	return 0;
}
 
static int beep_remove(struct platform_device *pdev)
{
    
	printk(KERN_ERR "driver remove ok!");
	return 0;
}
 
static struct platform_device_id p_id = {
    .name="bigbang",
};

static struct platform_driver beep_driver=
{
    .driver.name = "bigbang",
    /*.probe = beep_probe,*/
    .remove = beep_remove,
    .id_table = &p_id
};
 
static int beep_init(void)
{
	printk(KERN_ERR "driver module_init\n");
	
    return platform_driver_probe(&beep_driver, beep_probe);
	//return platform_driver_register(&beep_driver);
}
 
static void beep_exit(void)
{
	printk(KERN_ERR "driver module_exit\n");
	platform_driver_unregister(&beep_driver);
	
	return;
}
 
 
MODULE_LICENSE("GPL");
module_init(beep_init);
module_exit(beep_exit);
