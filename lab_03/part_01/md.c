#include <linux/init.h>
#include <linux/module.h>
#include <linux/sched.h>
#include <linux/init_task.h>
#include <linux/kernel.h>

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Sirotkina Polina");

static int __init md_init(void)
{
    struct task_struct *task = &init_task;
    do
    {
        printk(KERN_INFO "Process: pid = %d, comm = %s, state = %ld, prio = %d, ppid = %d, pcomm = %s.\n",
                            task->pid, task->comm, task->state, task->prio, 
                            task->parent->pid, task->parent->comm);
    } while ((task = next_task(task)) != &init_task);
    printk(KERN_INFO "Process(INIT current): pid = %d, comm = %s.\n", current->pid, current->comm);
    printk(KERN_INFO "Module was loaded.\n");
    return 0;
}

static void __exit md_exit(void)
{
    printk(KERN_INFO "Process(EXIT current): pid = %d, comm = %s.\n", current->pid, current->comm);
    printk(KERN_INFO "Module: Good Buy.\n");
}

module_init(md_init);
module_exit(md_exit);