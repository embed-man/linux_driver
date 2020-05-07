/* drivers/char/s3c2410_rtc.c
 *
 * Copyright (c) 2004 Simtec Electronics <linux@simtec.co.uk>
 *		      http://www.simtec.co.uk/products/SWLINUX/
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 *
 * S3C2410 Internal RTC Driver
 *
 *  Changelog:
 *	08-Nov-2004	BJD	Initial creation
 *	12-Nov-2004	BJD	Added periodic IRQ and PM code
 *	22-Nov-2004	BJD	Sign-test on alarm code to check for <0
 *	10-Mar-2005	LCVR	Changed S3C2410_VA_RTC to S3C24XX_VA_RTC
*/

#include <linux/module.h>
#include <linux/fs.h>
#include <linux/string.h>
#include <linux/init.h>
#include <linux/platform_device.h>
#include <linux/interrupt.h>
#include <linux/rtc.h>
#include <linux/bcd.h>
#include <linux/clk.h>

//#include <asm/hardware.h>
#include <asm/uaccess.h>
#include <asm/io.h>
#include <asm/irq.h>
//#include <asm/rtc.h>

//#include <asm/mach/time.h>

//#include <asm/arch/regs-rtc.h>

/* need this for the RTC_AF definitions */
#include <linux/mc146818rtc.h>

#undef S3C24XX_VA_RTC
#define S3C24XX_VA_RTC s3c2410_rtc_base

static void s3c2410_rtc_release(void)
{
}

static int s3c2410_rtc_remove(struct platform_device *dev)
{
	return 0;
}

static int s3c2410_rtc_probe(struct platform_device *pdev)
{
	struct resource *res;
	int ret;
    printk(KERN_ERR "probe function was fund...");
	/* find the IRQs */
	return ret;
}

#ifdef CONFIG_PM

/* S3C2410 RTC Power management control */

static int s3c2410_rtc_suspend(struct platform_device *pdev, pm_message_t state)
{
	return 0;
}

static int s3c2410_rtc_resume(struct platform_device *pdev)
{
	return 0;
}
#else
#define s3c2410_rtc_suspend NULL
#define s3c2410_rtc_resume  NULL
#endif

static struct platform_driver s3c2410_rtcdrv = {
	.probe		= s3c2410_rtc_probe,
	.remove		= s3c2410_rtc_remove,
	.suspend	= s3c2410_rtc_suspend,
	.resume		= s3c2410_rtc_resume,
	.driver		= {
		.name	= "test_platform_driver",
		.owner	= THIS_MODULE,
	},
};

static char __initdata banner[] = "S3C2410 RTC, (c) 2004 Simtec Electronics\n";

static int __init s3c2410_rtc_init(void)
{
	printk(banner);
	return platform_driver_register(&s3c2410_rtcdrv);
}

static void __exit s3c2410_rtc_exit(void)
{
	platform_driver_unregister(&s3c2410_rtcdrv);
}

module_init(s3c2410_rtc_init);
module_exit(s3c2410_rtc_exit);

MODULE_DESCRIPTION("S3C24XX RTC Driver");
MODULE_AUTHOR("Ben Dooks, <ben@simtec.co.uk>");
MODULE_LICENSE("GPL");
