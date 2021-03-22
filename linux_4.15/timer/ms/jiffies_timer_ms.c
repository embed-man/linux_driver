#include <linux/jiffies.h>//DO-->jiffies调用头文件
#include <linux/timer.h>  //DO-->timer_list结构体

static struct timer_list ms_timer;//DO-->定义timer_list结构体

static void ms_timer_handler(void)//DO-->定义定时器处理函数

{
    printk("DO_DEBUG----------->%s\n",__func__);
   // ms_timer.expires=jiffies+HZ;

    ms_timer.expires=jiffies+msecs_to_jiffies(10);
    ms_timer.function=&ms_timer_handler;
    add_timer(&ms_timer);

}

 

static int32_t xxx_init(void)
{
// hrtimer_init_module();
    init_timer(&ms_timer);                          //DO-->初始化定时器
    ms_timer.expires=jiffies+msecs_to_jiffies(10);  //DO-->定义中断时间：10ms进入中断
    //ms_timer.expires=jiffies+HZ;  
    //ms_timer.data=(unsigned long)ms_timer;//区分不同定时器，未验证
    ms_timer.function=&ms_timer_handler;            //DO-->定义定时器中断处理函数

    add_timer(&ms_timer);                           //DO-->增加注册定时器，使定时器生效

｝

