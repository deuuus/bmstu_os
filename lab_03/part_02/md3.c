#include "md.h"
#include <linux/init.h>
#include <linux/module.h>
#include <linux/slab.h>

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Sirotkina Polina");

static int __init md_init(void)
{
    printk("Module №3 was loaded.\n");
    printk("Module №3: data string exported from md1 : %s\n", md1_data);
    printk("Module №3: string returned md1_proc() is : %s\n", md1_proc());
    //printk("+ data number exported from md1 : %d\n", md1_number);
    //printk("+ md1_fact(5) call returns      : %d\n", md1_fact(5));
    return -1;
}

module_init(md_init);