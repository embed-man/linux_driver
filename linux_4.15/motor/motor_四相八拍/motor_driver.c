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
#include <linux/timer.h>
#include <linux/jiffies.h>
#include <linux/gpio.h>
#include <linux/delay.h>
#define MOTOR_MAJOR 99

struct  cdev dev;
struct class *device_class;
struct timer_list time;

/***********************************************proc fs********************************/
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
static int proc_show(struct seq_file *m, void *v)
{
    int len = 0;
    char *data = m->private;
    len += seq_printf(m,"proc_open:%s\t", data);
	return len;
}
static int open_proc(struct inode *inode, struct file *file)
{
	return single_open(file, proc_show, PDE_DATA(inode));
}
static int write_proc(struct file *file, const char __user *buffer,
			    size_t count, loff_t *data) {
	int command = 0;
	int i;
    printk(KERN_ERR "open_proc_write_data:%s\n", buffer);
	return count;
}
static const struct file_operations proc_motor_fops ={
	.read = seq_read,
	.open = open_proc,
	.write = write_proc,
	/*.llseek = seq_lseek,
	.release = single_release,*/
};


/******************************timer ms******************************************/
static unsigned long timer_cnt = 0;
static unsigned long timer_ms = 0;
static unsigned long delay_ms = 0;
static unsigned long rotate_direction = 0;
/*
 *橙
    黄
        粉
            蓝
 **/
static int init_one = 1;
void motor_eight_racket_reverse()
{
    if (init_one) {
        init_one = 0;
        gpio_request(GPIO_PB(9), "PB9");
        gpio_request(GPIO_PB(15), "PB15");
        gpio_request(GPIO_PB(16), "PB16");
        gpio_request(GPIO_PB(6), "PB6");
    }

    gpio_direction_output(GPIO_PB(9), 1);
    gpio_direction_output(GPIO_PB(15), 0);
    gpio_direction_output(GPIO_PB(16), 0);
    gpio_direction_output(GPIO_PB(6), 1);
    mdelay(delay_ms);

    gpio_direction_output(GPIO_PB(9), 0);
    gpio_direction_output(GPIO_PB(15), 0);
    gpio_direction_output(GPIO_PB(16), 0);
    gpio_direction_output(GPIO_PB(6), 1);
    mdelay(delay_ms);

    gpio_direction_output(GPIO_PB(9), 0);
    gpio_direction_output(GPIO_PB(15), 0);
    gpio_direction_output(GPIO_PB(16), 1);
    gpio_direction_output(GPIO_PB(6), 1);
    mdelay(delay_ms);

    gpio_direction_output(GPIO_PB(9), 0);
    gpio_direction_output(GPIO_PB(15), 0);
    gpio_direction_output(GPIO_PB(16), 1);
    gpio_direction_output(GPIO_PB(6), 0);
    mdelay(delay_ms);

    gpio_direction_output(GPIO_PB(9), 0);
    gpio_direction_output(GPIO_PB(15), 1);
    gpio_direction_output(GPIO_PB(16), 1);
    gpio_direction_output(GPIO_PB(6), 0);
    mdelay(delay_ms);

    gpio_direction_output(GPIO_PB(9), 0);
    gpio_direction_output(GPIO_PB(15), 1);
    gpio_direction_output(GPIO_PB(16), 0);
    gpio_direction_output(GPIO_PB(6), 0);
    mdelay(delay_ms);

    gpio_direction_output(GPIO_PB(9), 1);
    gpio_direction_output(GPIO_PB(15), 1);
    gpio_direction_output(GPIO_PB(16), 0);
    gpio_direction_output(GPIO_PB(6), 0);
    mdelay(delay_ms);

    gpio_direction_output(GPIO_PB(9), 1);
    gpio_direction_output(GPIO_PB(15), 0);
    gpio_direction_output(GPIO_PB(16), 0);
    gpio_direction_output(GPIO_PB(6), 0);
}
void motor_eight_racket()
{
    if (init_one) {
        init_one = 0;
        gpio_request(GPIO_PB(9), "PB9");
        gpio_request(GPIO_PB(15), "PB15");
        gpio_request(GPIO_PB(16), "PB16");
        gpio_request(GPIO_PB(6), "PB6");
    }
    gpio_direction_output(GPIO_PB(9), 1);
    gpio_direction_output(GPIO_PB(15), 0);
    gpio_direction_output(GPIO_PB(16), 0);
    gpio_direction_output(GPIO_PB(6), 0);
    mdelay(delay_ms);
    gpio_direction_output(GPIO_PB(9), 1);
    gpio_direction_output(GPIO_PB(15), 1);
    gpio_direction_output(GPIO_PB(16), 0);
    gpio_direction_output(GPIO_PB(6), 0);

    mdelay(delay_ms);
    gpio_direction_output(GPIO_PB(9), 0);
    gpio_direction_output(GPIO_PB(15), 1);
    gpio_direction_output(GPIO_PB(16), 0);
    gpio_direction_output(GPIO_PB(6), 0);
    mdelay(delay_ms);

    gpio_direction_output(GPIO_PB(9), 0);
    gpio_direction_output(GPIO_PB(15), 1);
    gpio_direction_output(GPIO_PB(16), 1);
    gpio_direction_output(GPIO_PB(6), 0);
    mdelay(delay_ms);

    gpio_direction_output(GPIO_PB(9), 0);
    gpio_direction_output(GPIO_PB(15), 0);
    gpio_direction_output(GPIO_PB(16), 1);
    gpio_direction_output(GPIO_PB(6), 0);
    mdelay(delay_ms);

    gpio_direction_output(GPIO_PB(9), 0);
    gpio_direction_output(GPIO_PB(15), 0);
    gpio_direction_output(GPIO_PB(16), 1);
    gpio_direction_output(GPIO_PB(6), 1);
    mdelay(delay_ms);

    gpio_direction_output(GPIO_PB(9), 0);
    gpio_direction_output(GPIO_PB(15), 0);
    gpio_direction_output(GPIO_PB(16), 0);
    gpio_direction_output(GPIO_PB(6), 1);
    mdelay(delay_ms);

    gpio_direction_output(GPIO_PB(9), 1);
    gpio_direction_output(GPIO_PB(15), 0);
    gpio_direction_output(GPIO_PB(16), 0);
    gpio_direction_output(GPIO_PB(6), 1);
}
void timer_rotate_motor(unsigned long argv)
{
    if (timer_cnt > 0) {
        timer_cnt --;
        if (rotate_direction) {
            motor_eight_racket();
        } else {
            motor_eight_racket_reverse();
        }
        mod_timer(&time, jiffies + msecs_to_jiffies(argv));//add_timer
    } else {
        del_timer(&time);
        printk(KERN_ERR "timer destroy:%d\n", argv);
    }
    //printk(KERN_ERR "timer is down:%ld\n", argv);
}
void timer_init_ms()
{
    printk(KERN_ERR "timer start\n");
    init_timer(&time);
    time.data = timer_ms;
    time.expires = jiffies + msecs_to_jiffies(time.data);//10ms
    time.function = &timer_rotate_motor;
    add_timer(&time);
}
void timer_del_ms()
{
    del_timer(&time);
}
/*********************************************************************************************/
int motor_open(struct inode *inode, struct file *filp)
{
    /*将设备结构体指针赋值给文件私有数据指针*/
    printk(KERN_ERR "motor open\n");
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

    printk(KERN_ERR "ioctl cmd:%d, value:%d\n", cmd, arg);
    switch (cmd)
    {
        case 1://设置定时器执行的次数
            timer_cnt = arg;
            break;
        case 2://设置定时器多久被执行
            timer_ms = arg;
        case 3://每个脉冲的延时us
            delay_ms = arg;
        case 4://启动0:   1:
            rotate_direction = arg & 0x01;
            timer_init_ms();
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

    printk(KERN_ERR "motor read\n");
    return ret;
}

/*写函数*/
static ssize_t motor_write(struct file *filp, const char __user *buf,
  size_t size, loff_t *ppos)
{
    int ret = 0; 
    unsigned long p =  *ppos;
    unsigned int count = size;
    printk(KERN_ERR "motor write\n");
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
	struct proc_dir_entry *p;
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
    printk(KERN_ERR "enter probe\n");

	p = p_proc_mkdir("motor");
	if (!p) {
		printk("create_proc_entry for common reset failed.\n");
		return -ENODEV;
	}
    proc_create_data("operation_motor", 0444,p,&proc_motor_fops, "just test proc_create data");
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
static void __exit motor_exit(void)
{
    //platform_driver_unregister($motor_pdrv);
    //platform_device_unregister($motor_pdev);
    return ;
}
module_init(motor_init);
module_exit(motor_exit);
MODULE_LICENSE("GPL");
