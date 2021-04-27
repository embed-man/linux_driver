/*
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 */
#include <linux/kernel.h>
#include <linux/delay.h>
#include <linux/proc_fs.h>
#include <linux/platform_device.h>
#include <linux/seq_file.h>
#include <linux/cdev.h>
#include <linux/module.h>
#include <linux/export.h>
#include <linux/ioport.h>
#define MOTOR_MAJOR 99

static int motor_probe(struct platform_device *pdev)
{
    int res_num = 0;
    struct resource *res = NULL;    
    res_num = pdev->num_resources;
    printk(KERN_ERR "resource num is %d\n", res_num);
    int i = 0;
    for (i = 0; i < res_num; i++) {
        //platform_get_resource(pdev, res, 1);
        printk(KERN_ERR "resource index :%d,start:0x%lx,end:0x%lx\n", i, pdev->resource[i].start, pdev->resource[i].end);
    }
	return 0;
}
int motor_suspend(struct platform_device *pdev, pm_message_t state)
{

	return 0;
}

void motor_shutdown(struct platform_device *pdev)
{
}

int motor_resume(struct platform_device *pdev)
{

	return 0;
}

typedef struct _tag_test_plat_data {
    int id;
    int value;
}test_plat_data;

test_plat_data plat_data = {
    .id = 1,
    .value = 2,
};

struct resource res[2] = {

    [0] = {
        .start = 0x00000000,
        .end = 0x00000001,
        .flags = IORESOURCE_MEM
    },
    [1] = {
        .start = 0x00000002,
        .end = 0x00000003,
        .flags = IORESOURCE_DMA,
    }
};
static struct platform_device motor_pdev = {
    .name = "motor",
    .resource   = res,
    .num_resources = sizeof(res) / sizeof(struct resource),
    .dev = {
        .init_name = "motor",
        .platform_data = &plat_data,
    }
    
};
static struct platform_driver motor_pdrv = {
	.probe		= motor_probe,
	.shutdown	= motor_shutdown,
	.suspend	= motor_suspend,
	.resume		= motor_resume,
	.driver		= {
		.name	= "motor",
		.owner	= THIS_MODULE,
	},
};

static int __init motor_init(void)
{
	platform_driver_register(&motor_pdrv);
	platform_device_register(&motor_pdev);
	return 0;
}
#if 1
static void __exit motor_exit(void)
{
    //platform_driver_unregister($motor_pdrv);
    //platform_device_unregister($motor_pdev);
    return ;
}
#endif
module_init(motor_init);
module_exit(motor_exit);
MODULE_LICENSE("GPL");
