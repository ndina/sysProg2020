struct task_struct *kthread_create_on_node(int (*threadfn)(void *data),
 void *data,
 int node,
 const char namefmt[], ...);

#define kthread_create(threadfn, data, namefmt, arg...) 
 kthread_create_on_node(threadfn, data, NUMA_NO_NODE, namefmt, ##arg)


struct task_struct *kthread_create_on_cpu(int (*threadfn)(void *data),
 void *data,
 unsigned int cpu,
 const char *namefmt);

#define kthread_run(threadfn, data, namefmt, ...) 
({ 
 struct task_struct *__k 
 = kthread_create(threadfn, data, namefmt, ## __VA_ARGS__); 
 if (!IS_ERR(__k)) 
 wake_up_process(__k); 
 __k; 
})