#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/hrtimer.h>
#include <linux/kthread.h>
#include <linux/delay.h>
#include <linux/jiffies.h>
#include <linux/unistd.h>
#include <linux/sched.h>
#include <linux/ktime.h>
#include <linux/timekeeping.h>

MODULE_LICENSE("GPL");

#define INTERVAL_BETWEEN_CALLBACKS (100 * 10000000LL)


int counter = 1;
struct timespec tv;

static struct hrtimer timer;
static ktime_t kt;

static s64 starttime_ns;

int gen_fib(int n){
    if (n <= 1) return n;
    return gen_fib(n-1) + gen_fib(n-2);
}

void print_current_time(void) {
   getnstimeofday(&tv);
   long long hours = (tv.tv_sec/3600 + 6)%24;
   long long minutes = (tv.tv_sec/60)%60;
   long long seconds = tv.tv_sec%60; 
   pr_info("Current time: %lld:%lld:%lld", hours, minutes, seconds);
}

static enum hrtimer_restart my_hrtimer_callback( struct hrtimer *timer )
{
    pr_info("############################################################");
	printk("I'm in hrtimer handler. Counter now is %d\r\n", counter);
    print_current_time();
    printk("Current fibonacci number: %d", gen_fib(counter));
    s64 now_ns = ktime_to_ns(ktime_get());
    kt = ktime_set(0, gen_fib(counter)*INTERVAL_BETWEEN_CALLBACKS); 

    hrtimer_forward(timer, timer->_softexpires, kt);
    
    counter++;
    return HRTIMER_RESTART;
}
 
static int init_module_hrtimer( void )
{
	printk("hrtimer: installing module...\n");
	kt = ktime_set(0, INTERVAL_BETWEEN_CALLBACKS);
    hrtimer_init(&timer, CLOCK_MONOTONIC, HRTIMER_MODE_REL);
    hrtimer_start(&timer, kt, HRTIMER_MODE_REL);
    timer.function = my_hrtimer_callback;
    return 0;
}
 

static void cleanup_module_hrtimer( void )
{
	int ret;
	ret = hrtimer_cancel( &timer );
	if (ret)
	printk("hrtimer: The timer was still in use...\n");
	printk("hrtimer: HR Timer module uninstalling\n");
}
 
module_init(init_module_hrtimer);
module_exit(cleanup_module_hrtimer);