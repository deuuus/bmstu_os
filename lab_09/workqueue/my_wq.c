#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/interrupt.h>
#include <linux/init.h>
#include <linux/proc_fs.h>
#include <linux/seq_file.h>
#include <linux/workqueue.h>
#include <linux/slab.h>
#include <linux/smp.h>
#include <linux/delay.h>
#include <asm/io.h>

#define MAX_BUF_SIZE 200 * PAGE_SIZE

#define KBD_DATA_REG 0x60
#define KBD_SCANCODE_MASK 0x7f
#define KBD_STATUS_MASK 0x80

#define WQ_NAME_LEN 24
#define IRQ 1
static int devID;

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Sirotkina");

static char *ascii_map[] = {
    "[ESC]", "1", "2", "3", "4", "5", "6", "7", "8", "9", "0", "-", "=", "bs", "[Tab]", "Q",
    "W", "E", "R", "T", "Y", "U", "I", "O", "P", "[", "]", "[Enter]", "[CTRL]", "A", "S", "D",
    "F", "G", "H", "J", "K", "L", ";", "\'", "`", "[LShift]", "\\", "Z", "X", "C", "V", "B", "N", "M",
    ",", ".", "/", "[RShift]", "[PrtSc]", "[Alt]", " ", "[Caps]", "F1", "F2", "F3", "F4", "F5",
    "F6", "F7", "F8", "F9", "F10", "[Num]", "[Scroll]", "[Home(7)]", "[Up(8)]", "[PgUp(9)]", "-",
    "[Left(4)]", "[Center(5)]", "[Right(6)]", "+", "[End(1)]", "[Down(2)]", "[PgDn(3)]", "[Ins]", "[Del]"};

static struct workqueue_struct *my_workqueue;
static struct work_struct *work1, *work2;

int define_irq = 1;
char scancode;
int status;
char *key;

int count_work_02 = 0;

static void work1_handler(struct work_struct *work)
{
    status = scancode & KBD_STATUS_MASK;
    if (!status)
    {   
        key = ascii_map[(scancode & KBD_SCANCODE_MASK) - 1];
        printk(KERN_INFO "+ [work1_handler]: Key pressed - %s.\n", key);
        msleep(10);
    }
}

static void work2_handler(struct work_struct *work)
{
    count_work_02++;
    printk(KERN_INFO "+ [work2_handler]: Key was clicked. Count: %d.\n", count_work_02);
}

static irqreturn_t handler(int irq, void *dev)
{
    if (irq == define_irq)
    {
        int ret;
        scancode = inb(KBD_DATA_REG);

        if (my_workqueue)
        {
            work1 = kmalloc(sizeof(struct work_struct), GFP_KERNEL);
            if (work1)
            {
                INIT_WORK(work1, work1_handler);
                ret = queue_work(my_workqueue, (struct work_struct *)work1);
            }

            work2 = kmalloc(sizeof(struct work_struct), GFP_KERNEL);
            if (work2)
            {
                INIT_WORK(work2, work2_handler);
                ret = queue_work(my_workqueue, (struct work_struct *)work2);
            }
        }

        return IRQ_HANDLED;
    }
    else
    {
        return IRQ_NONE;
    }
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
