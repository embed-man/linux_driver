/*
 * sensor_info.c
 *
 * Copyright (C) 2012 Ingenic Semiconductor Co., Ltd.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 */

#include <linux/init.h>
#include <linux/module.h>
#include <linux/slab.h>
#include <linux/i2c.h>
#include <linux/delay.h>
#include <linux/gpio.h>
#include <linux/clk.h>
#include <linux/cdev.h>
#include <linux/fs.h>
#include <linux/unistd.h>
#include <linux/types.h>
#include <linux/ioctl.h>
#include <linux/miscdevice.h>
#include <linux/mutex.h>

#include <linux/module.h>
#include <linux/proc_fs.h>
#include <linux/seq_file.h>


static struct mutex g_mutex;
static long misc_ioctl(struct file *filp, unsigned int cmd, unsigned long arg)
{
	int ret = 0;
	int32_t data;

	mutex_lock(&g_mutex);

	mutex_unlock(&g_mutex);
	return ret;
}
static int misc_open(struct inode *inode, struct file *filp)
{
    printk(KERN_ERR "misc open....\n");
	return 0;
}
static int misc_release(struct inode *inode, struct file *filp)
{
	printk (KERN_ERR "misc misc_release\n");
	return 0;
}

static ssize_t misc_read(struct file *filp, char __user *buf, size_t count, loff_t *f_pos)
{
	return 0;
}

static struct file_operations misc_fops =
{
	.owner = THIS_MODULE,
	.read = misc_read,
	.unlocked_ioctl = misc_ioctl,
	.open = misc_open,
	.release = misc_release,
};

static struct miscdevice misc_misc = {
	.minor = MISC_DYNAMIC_MINOR,
	.name = "miscDemo",
	.fops = &misc_fops,
};


static __init int init_misc(void)
{
	int ret = 0;
	mutex_init(&g_mutex);

	ret = misc_register(&misc_misc);
	/* printk("##### g_sensor_id = %d\n", g_sensor_id); */
	return ret;

}

static __exit void exit_misc(void)
{
	misc_deregister(&misc_misc);
}

module_init(init_misc);
module_exit(exit_misc);

MODULE_DESCRIPTION("misc driver demo");
MODULE_LICENSE("GPL");
