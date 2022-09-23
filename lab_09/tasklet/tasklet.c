#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/interrupt.h>
#include <linux/time.h>

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Sirotkina");

#define KEYB_IRQ 1

char my_tasklet_data[] = "lab_09";
struct tasklet_struct my_tasklet;

void my_tasklet_func(unsigned long data)
{
  printk(KERN_DEBUG "+ [my_tasklet_func]: state - %ld, count - %d, data - %s.\n", my_tasklet.state, my_tasklet.count, my_tasklet.data);
}

irqreturn_t irq_handler(int irq, void *dev)
{
  if(irq == KEYB_IRQ)
  {
    tasklet_schedule(&my_tasklet);
    printk(KERN_DEBUG "+ [irq_handler]: tasklet scheduled.\n");
    return IRQ_HANDLED;
  }
  else
    return IRQ_NONE;
}

static int __init my_module_init(void)
{
  printk(KERN_DEBUG "+ [my_module_init]: module loaded.\n");

  int ret = request_irq(KEYB_IRQ, (irq_handler_t)irq_handler, IRQF_SHARED, "my_keyboard_interrupt", (void *)(irq_handler));

  if (ret != 0)
  {
    printk(KERN_ERR "+ [my_module_init]: keyboard IRQ handler wasn't registered.");
    return ret;
  }

  printk(KERN_DEBUG "+ [my_module_init]: keyboard IRQ handler was registered.");

  tasklet_init(&my_tasklet, my_tasklet_func, &my_tasklet_data);
  return ret;
}

static void __exit my_module_exit(void)
{
  free_irq(KEYB_IRQ, (void *)(irq_handler));
  tasklet_disable(&my_tasklet);
  tasklet_kill(&my_tasklet);
  printk(KERN_DEBUG "+ [my_module_exit]: module unloaded.\n");
}

module_init(my_module_init);
module_exit(my_module_exit);