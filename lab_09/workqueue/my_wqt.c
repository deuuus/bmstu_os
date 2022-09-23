#include <linux/interrupt.h>
#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/proc_fs.h>
#include <linux/seq_file.h>
#include <linux/workqueue.h>
#include <linux/delay.h>

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Sirotkina");

static char *ascii_map[] = {
    "[ESC]", "1", "2", "3", "4", "5", "6", "7", "8", "9", "0", "-", "=", "bs", "[Tab]", "Q",
    "W", "E", "R", "T", "Y", "U", "I", "O", "P", "[", "]", "[Enter]", "[CTRL]", "A", "S", "D",
    "F", "G", "H", "J", "K", "L", ";", "\'", "`", "[LShift]", "\\", "Z", "X", "C", "V", "B", "N", "M",
    ",", ".", "/", "[RShift]", "[PrtSc]", "[Alt]", " ", "[Caps]", "F1", "F2", "F3", "F4", "F5",
    "F6", "F7", "F8", "F9", "F10", "[Num]", "[Scroll]", "[Home(7)]", "[Up(8)]", "[PgUp(9)]", "-",
    "[Left(4)]", "[Center(5)]", "[Right(6)]", "+", "[End(1)]", "[Down(2)]", "[PgDn(3)]", "[Ins]", "[Del]"};

char scancode;
int status;
char *key;

#define IRQ 1
static int devID;

int count_work_02 = 0;

struct workqueue_struct *my_workqueue;

void queue_f1(struct work_struct *work)
{
    printk(KERN_INFO "+ [queue_f1]: Key was clicked. Sleep was called.\n");
    msleep(10);
}

void queue_f2(struct work_struct *work)
{
    count_work_02++;
    printk(KERN_INFO "+ [queue_f2]: Key was clicked. Count: %d.\n", count_work_02);
}

struct work_struct work_01;
struct work_struct work_02;

irqreturn_t handler(int irq, void *dev)
{
    if (irq == IRQ)
    {
        queue_work(my_workqueue, &work_01);
        queue_work(my_workqueue, &work_02);
        printk(KERN_INFO "+ [handler]: move work to queue.\n");
        return IRQ_HANDLED;
    }
    printk(KERN_INFO "+ [handler]: can't move work to queue.\n");
    return IRQ_NONE;
}

static int __init work_queue_init(void)
{
    int ret = request_irq(IRQ, handler, IRQF_SHARED, "my_interrupt", &devID);
    if (ret)
    {
        printk(KERN_ERR "+ [workqueue_init]: request_irq failed.\n");
        return ret;
    }

    if (!(my_workqueue = create_workqueue("workqueue")))
    {
        free_irq(IRQ, &devID);
        printk(KERN_INFO "+ [workqueue_init]: create_workqueue failed.");
        return -ENOMEM;
    }

    INIT_WORK(&work_01, queue_f1);
    INIT_WORK(&work_02, queue_f2);

    printk(KERN_INFO "+ [workqueue_init]: module loaded.\n");
    return 0;
}

static void __exit work_queue_exit(void)
{
    flush_workqueue(my_workqueue);
    destroy_workqueue(my_workqueue);
    free_irq(IRQ, &devID);
    remove_proc_entry("workqueue", NULL);
    printk(KERN_INFO "+ [workqueue_exit]: module unloaded.\n");
}

module_init(work_queue_init) 
module_exit(work_queue_exit)