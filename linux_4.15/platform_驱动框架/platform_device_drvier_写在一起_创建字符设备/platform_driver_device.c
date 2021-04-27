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
#define MOTOR_MAJOR 99

struct  cdev dev;
struct class *device_class;


static struct proc_dir_entry *proc_root;
struct proc_dir_entry * p_proc_mkdir(char *s)
{
	struct proc_dir_entry *p;
	if(!proc_root) {
		proc_root = proc_mkdir("motor_info", 0);
		if(!proc_root)
			return NULL;
	}
	p = proc_mkdir(s,proc_root);
	return p;
}
#if 0
static const struct file_operations motor_fops ={
	.read = seq_read,
	.open = reset_proc_open,
	.write = reset_write_proc,
	.llseek = seq_lseek,
	.release = single_release,
};
#endif
int motor_open(struct inode *inode, struct file *filp)
{
    /*将设备结构体指针赋值给文件私有数据指针*/
    filp->private_data = NULL;
    return 0;
}
/*文件释放函数*/
int motor_release(struct inode *inode, struct file *filp)
{
    return 0;
}

/* ioctl设备控制函数 */
static long motor_ioctl(struct file *filp, unsigned
  int cmd, unsigned long arg)
{

    switch (cmd)
    {
        case 1:
            break;
        default:
            return - EINVAL;
    }
    return 0;
}

/*读函数*/
static ssize_t motor_read(struct file *filp, char __user *buf, size_t size,
  loff_t *ppos)
{
    int ret = 0;
    unsigned long p =  *ppos;
    unsigned int count = size;

    return ret;
}

/*写函数*/
static ssize_t motor_write(struct file *filp, const char __user *buf,
  size_t size, loff_t *ppos)
{
    int ret = 0; 
    unsigned long p =  *ppos;
    unsigned int count = size;
    return ret;
}
static const struct file_operations motor_fops = {
    .owner = THIS_MODULE,
    .open = motor_open,
    .read = motor_read,
    .write = motor_write,
    .unlocked_ioctl = motor_ioctl,
};

static int motor_probe(struct platform_device *pdev)
{
    dev_t dev_num;
    int ret = -1;
    if (MOTOR_MAJOR) {
        ret = register_chrdev_region(MKDEV(MOTOR_MAJOR, 0), 1, "motor_dev");
    } else {
        ret = alloc_chrdev_region(&dev_num, 0, 1, "motor_dev");
    }
    if (ret < 0) {
        printk(KERN_ERR "register_chrdev_region fail\n");
        return -1;
    }
    cdev_init(&dev, &motor_fops);
    dev.owner = THIS_MODULE;
    dev.ops = &motor_fops;
    cdev_add(&dev, MKDEV(MOTOR_MAJOR, 0), 1);
    //--------------
    device_class = class_create(THIS_MODULE, "motor_dev");
    device_create(device_class, NULL, MKDEV(MOTOR_MAJOR, 0), NULL, "motor_dev");
    //--------------
	struct proc_dir_entry *p;
    printk(KERN_ERR "enter probe\n");

	p = p_proc_mkdir("motor");
	if (!p) {
		printk("create_proc_entry for common reset failed.\n");
		return -ENODEV;
	}
       // proc_create_data("operation_motor", 0444,p,&motor_fops,motor);
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

static struct platform_device motor_pdev = {
	.name		= "motor",
    .dev        = {

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
