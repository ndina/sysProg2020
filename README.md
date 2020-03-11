# Basic implementations of kernel threads


## Prerequisites

Make sure you have gcc compiler on your system


## Build Instructions

1. /*kthreadd creation code (thread.c) */( * We need to spawn init first so that it obtains pid 1, however
 * the init task will end up wanting to create kthreads,)
    1.1   The boot idle thread must execute schedule() at least once to get things moving
2.  * kthread_create - create a kthread on the current node
    @threadfn: the function to run in the thread
    @data: data pointer for @threadfn()
    @namefmt: printf-style format string for the thread name
    @...: arguments for @namefmt



## Author

*  **[Dina Nassyrkhan](https://github.com/ndina)**


