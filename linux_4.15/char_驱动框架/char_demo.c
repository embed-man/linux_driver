#include <linux/module.h>
#include <linux/types.h>
#include <linux/fs.h>
#include <linux/errno.h>
#include <linux/mm.h>
#include <linux/sched.h>
#include <linux/init.h>
#include <linux/cdev.h>
#include <linux/device.h>
#include <asm/io.h>
#include <asm/uaccess.h>
#include <linux/slab.h>
#include <linux/ioctl.h>
#include <linux/device.h>
#include <linux/uaccess.h>


/*�豸����ģ����غ���*/
struct class *my_class;
static struct cdev dev;

#define GLOBALMEM_MAJOR 90    /*Ԥ���char�����豸��*/

static int char_major = GLOBALMEM_MAJOR;

/*�ļ��򿪺���*/
int char_open(struct inode *inode, struct file *filp)
{
  /*���豸�ṹ��ָ�븳ֵ���ļ�˽������ָ��*/
  //filp->private_data = ;
  return 0;
}
/*�ļ��ͷź���*/
int char_release(struct inode *inode, struct file *filp)
{
  return 0;
}

/* ioctl�豸���ƺ��� */
static long char_ioctl(struct file *filp, unsigned
  int cmd, unsigned long arg)
{
  switch (cmd) {
    default:
      return  - EINVAL;
  }
  return 0;
}

/*������*/
static ssize_t char_read(struct file *filp, char __user *buf, size_t size,
  loff_t *ppos)
{
  unsigned long p =  *ppos;
  unsigned int count = size;
  int ret = 0;

  return ret;
}

/*д����*/
static ssize_t char_write(struct file *filp, const char __user *buf,
  size_t size, loff_t *ppos)
{
  unsigned long p =  *ppos;
  unsigned int count = size;
  int ret = 0;

  return ret;
}

static loff_t char_llseek(struct file *filp, loff_t offset, int orig)
{
  loff_t ret = 0;
  switch (orig) {
    default:
      ret =  - EINVAL;
      break;
  }
  return ret;
}

/*�ļ������ṹ��*/
static const struct file_operations char_fops =
{
  .owner = THIS_MODULE,
  .llseek = char_llseek,
  .read = char_read,
  .write = char_write,
  .unlocked_ioctl = char_ioctl,
  .open = char_open,
  .release = char_release,
};


int char_init(void)
{
  int result, err;
  dev_t devno = MKDEV(char_major, 0);
  /* �����豸��*/
  if (char_major) {
    result = register_chrdev_region(devno, 1, "charDemo2");
  } else {/* ��̬�����豸�� */
    result = alloc_chrdev_region(&devno, 0, 1, "charDemo2");
    char_major = MAJOR(devno);
  }  
  if (result < 0) {
    return result;
  }
    
  cdev_init(&dev, &char_fops);
  dev.owner = THIS_MODULE;
  dev.ops = &char_fops;
  err = cdev_add(&dev, devno, 1);
  if (err) {
  }
  
  my_class = class_create(THIS_MODULE, "charDemo1");/*��/sys/class�´���һ��charDemo1��Ŀ¼*/
  device_create(my_class, NULL, MKDEV(90, 0), NULL, "charDemo2");/*�����洴����Ŀ¼�´���charDemo2�ļ�,ͬʱ��/dev/�´����豸�ļ���*/
  printk(KERN_INFO "class create....");
  return 0;

fail_malloc: 
  unregister_chrdev_region(devno, 1);
  return result;
}

/*ģ��ж�غ���*/
void char_exit(void)
{
  cdev_del(&dev);   /*ע��cdev*/
  unregister_chrdev_region(MKDEV(char_major, 0), 1); /*�ͷ��豸��*/
  
  device_destroy(my_class, MKDEV(90,0));
  class_destroy(my_class);
  printk(KERN_INFO "class destroy....");
}

MODULE_AUTHOR("Song Baohua");
MODULE_LICENSE("Dual BSD/GPL");

module_param(char_major, int, S_IRUGO);

module_init(char_init);
module_exit(char_exit);
