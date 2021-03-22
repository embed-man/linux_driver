//proc文件系统的编程模型


############################################创建文件夹########################################
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
	struct proc_dir_entry *p;
    printk(KERN_ERR "enter probe\n");

	p = p_proc_mkdir("motor");

############################################创建文件########################################
//创建文件,之后可以对该文件进行读写操作, 这样就赋予了该文件驱动的功能,应用层传值，底层控制
static int proc_show(struct seq_file *m, void *v)
{
    int len = 0;
    char *data = m->private;
    len += seq_printf(m,"%s\t", data);
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
proc_create_data("operation_motor"/*文件名*/, 0444,p,&proc_motor_fops, "just test proc_create data"/*传给函数的值,可以不要, proc_show中的m->private*/);
