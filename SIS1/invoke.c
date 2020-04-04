/* kernel/kthread.c */
static struct task_struct *__kthread_create_on_node(int (*threadfn)(void *data),
 void *data, int node,
 const char namefmt[],
 va_list args)
{
	 DECLARE_COMPLETION_ONSTACK(done);
	 struct task_struct *task;
	struct kthread_create_info *create = kmalloc(sizeof(*create),
	 GFP_KERNEL);

	 if (!create)
	 return ERR_PTR(-ENOMEM);
	create->threadfn = threadfn;
	 create->data = data;
	 create->node = node;
	 create->done = &done;

	 spin_lock(&kthread_create_lock);
	list_add_tail(&create->list, &kthread_create_list);
	 spin_unlock(&kthread_create_lock);

	wake_up_process(kthreadd_task);
 /*
 * Wait for completion in killable state, for I might be chosen by
 * the OOM killer while kthreadd is trying to allocate memory for
 * new kernel thread.
 */
 if (unlikely(wait_for_completion_killable(&done))) {
 /*
 * If I was SIGKILLed before kthreadd (or new kernel thread)
 * calls complete(), leave the cleanup of this structure to
 * that thread.
 */
 if (xchg(&create->done, NULL))
 return ERR_PTR(-EINTR);
/*
 * kthreadd (or new kernel thread) will call complete()
 * shortly.
 */
 wait_for_completion(&done); // wakeup on completion of thread creation.
 }
...
...
...
}

	struct task_struct *kthread_create_on_node(int (*threadfn)(void *data),
	 void *data, int node,
	 const char namefmt[],
	 ...)
	{
	 struct task_struct *task;
	 va_list args;

	 va_start(args, namefmt);
	task = __kthread_create_on_node(threadfn, data, node, namefmt, args);
	 va_end(args);

	 return task;
}