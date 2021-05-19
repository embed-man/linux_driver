/*定义tasklet和底半部函数并关联*/
void xxx_do_tasklet(unsigned long);
DECLARE_TASKLET(xxx_tasklet,xxx_do_tasklet,0);
 
 
/*中断处理底半部*/
void xxx_do_tasklet(unsigned long)
 
{
 
...
 
}
 
/*中断处理顶半部*/
irqreturn_t xxx_interrupt(int irq,void *dev_id)
 
{
 
...
 
    tasklet_schedule(&xxx_tasklet);  //调度底半部
 
...
 
}
 
/*设备驱动模块加载函数*/
int __init xxx_init(void)
 
{
    /*申请中断*/
    result = request_irq(xxx_irq, xxx_interrupt, IRQF_DISABLED,"xxx",NULL);
    return IRQ_HANDLED;
 
}
 
/*设备驱动模块卸载函数*/
void __exit xxx_exit(void)
 
{
/*释放中断*/
free_irq(xxx_irq,xxx_interrupt);
}
--------------------------------------------------------------------------------------------
/*定义工作队列和关联函数*/
struct work_struct(unsigned long);
void xxx_do_work(unsigned long);
 
/*中断处理底半部*/
void xxx_do_work(unsigned long)
{
...
}
 
/*中断处理顶半部*/
irqreturn_t xxx_interrupt(int irq,void *dev_id)
{
...
    schedule_work(&my_wq); //调度底半部
...
 
    return IRQ_HANDLED;
}
 
/*设备驱动模块加载函数*/
int xxx_init(void)
{
...
    /*申请中断*/
    result = request_irq(xxx_irq, xxx_interrupt, IRQF_DISABLED,"xxx",NULL);
...
    /*初始化工作队列*/
    INIT_WORK(&my_wq,(void (*)(void *))xxx_do_work,NULL);
}
 
/*设备驱动模块卸载函数*/
void xxx_exit(void)
{
...
    /*释放中断*/
    free_irq(xxx_irq,xxx_interrupt);
...
}
