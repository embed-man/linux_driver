#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/proc_fs.h>
#include <linux/sched.h>
#include <asm/uaccess.h>
#define DEBUG 
#include <linux/printk.h>
#define STRINGLEN 1024
 
char global_buffer[STRINGLEN];
 
struct proc_dir_entry *example_dir, *hello_file;
 
ssize_t proc_read_hello(struct file *f, char __user *buf, size_t size, loff_t *offset)
{
    int len;
    raw_copy_to_user(buf, global_buffer, size);
    return size;
}
 
ssize_t  proc_write_hello(struct file *f, const char __user *buf, size_t size, loff_t *offset)
{
    raw_copy_from_user(global_buffer, buf, size);
    global_buffer[size] = '\0';
    return size;
}
 

static int proc_show_ver(struct seq_file *file, void *v)
{
    int cnt = 0;
#if 0
    seq_printf(file, "Driver: %s\n", DRV_NAME);    /*函数seq_printf是最常用的输出函数，它用于把给定参数按照给定的格式输出到seq_file文件*/
    seq_printf(file, "Author: %s\n", AUTH);
    seq_printf(file, "Version: %s\n", VER);
    seq_printf(file, "Time: %s %s\n", __DATE__, __TIME__);
#endif
    return cnt;
}

static int proc_key_open(struct inode *inode, struct file *file)
{
    //single_open(file, proc_show_ver, NULL);  /*将proc_show_ver 和 seq_file文件关联起来*/
    return 0;
}

static struct file_operations proc_file_ops = {
    .owner   = THIS_MODULE,
    .open     = proc_key_open, 
    .read     = proc_read_hello/*seq_read如果这儿填seq_read则会在read的时候调用proc_show_ver*/,     /*定义在linux/seq_file.c*/
    .write    = proc_write_hello,
    /*.release = single_release,*/  /*定义在linux/seq_file.c*/
};



static int __init proc_test_init(void)
{
    pr_debug("proc_test_init...\n");
    pr_err("pr_err proc_test_init...\n");
    example_dir = proc_mkdir("proc_test", NULL);
    hello_file = proc_create_data("test_hello", 0, NULL, &proc_file_ops, NULL);
    strcpy(global_buffer, "hello world");
    return 0;
}
 
static void __exit proc_test_exit(void)
{
    pr_debug("proc_test_exit...\n");
    pr_err("pr_err proc_test_exit...\n");
    remove_proc_entry("test_hello", example_dir);
    remove_proc_entry("proc_test", NULL);
}

module_init(proc_test_init);
module_exit(proc_test_exit);

MODULE_DESCRIPTION("proc driver demo");
MODULE_LICENSE("GPL");
