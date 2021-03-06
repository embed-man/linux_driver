#include <linux/dma-mapping.h> //DO-->hrtimer包含以下三个头文件 /* DMA APIs             */   
#include <linux/hrtimer.h>  

#include <linux/time.h>           /* struct timespec    */

#define KER_PRINT(fmt, ...) printk("<ker-driver>"fmt, ##__VA_ARGS__);   
static struct hrtimer vibe_timer; 

static struct work_struct vibe_work;  
static int value = 2000;   /*注：以毫秒ms为单位 Time out setting,2 seconds */

static enum hrtimer_restart vibrator_timer_func(struct hrtimer *timer)  //DO-->回调函数，中断时调用 
{    

    struct timespec uptime;

    do_posix_clock_monotonic_gettime(&uptime);  
    KER_PRINT("Time:%lu.%02lu\n", 

            (unsigned long) uptime.tv_sec,  
            (uptime.tv_nsec / (NSEC_PER_SEC / 1000))); 

    KER_PRINT("vibrator_timer_func\n");   
    schedule_work(&vibe_work);  
    return HRTIMER_NORESTART; 

}  
static void vibe_work_func(struct work_struct *work)  //DO-->工作队列函数
{ 

    KER_PRINT("'vibe_work_func'-->work\n");  
   // msleep(50); /* CPU sleep */

    vibe_timer.function = vibrator_timer_func;  
    hrtimer_start(&vibe_timer, 

        ktime_set(value / 1000, (value % 1000) * 1000000),HRTIMER_MODE_REL);  
} 

static void ker_driver_init(void)                        //DO-->hrtimer高精度定时器初始化函数
{ 

    struct timespec uptime; 

    KER_PRINT("ker_driver_init\n");  
    hrtimer_init(&vibe_timer, CLOCK_MONOTONIC, HRTIMER_MODE_REL);  //DO-->hrtimer定时器初始化

    vibe_timer.function = vibrator_timer_func;                     //DO-->hrtimer定时器回调函数
    hrtimer_start(&vibe_timer,  

        ktime_set(value / 1000, (value % 1000) * 1000000),HRTIMER_MODE_REL);  //DO-->hrtimer定时器时间初始化，其中ktime_set（秒，纳秒）

    do_posix_clock_monotonic_gettime(&uptime);    //线程建立时间，用于比较看（定时器）此时时间
    KER_PRINT("Time:%lu.%02lu\n", 

            (unsigned long) uptime.tv_sec,  
            (uptime.tv_nsec / (NSEC_PER_SEC / 1000))); 

    INIT_WORK(&vibe_work, vibe_work_func);  /* Intialize the work queue */  //初始化工作队列

} 

 

static int32_t xxxx_init(void)

{

    ker_driver_init();

    ....

}
