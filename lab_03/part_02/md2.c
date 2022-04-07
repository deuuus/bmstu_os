#include <linux/init.h>
#include <linux/module.h>
#include <linux/slab.h>
#include "md.h"

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Sirotkina Polina");

static int __init md_init(void)
{
    printk("Module №2 was loaded.\n");
    printk("Module №2: data string exported from md1 : %s\n", md1_data);
    printk("Module №2: string returned md1_proc() is : %s\n", md1_proc());
    printk("Module №2: string returned md1_ocal() is : %s\n", md1_local());
    //printk("+ data number exported from md1 : %d\n", md1_number);
    //printk("+ string returned md1_noexport() is : %s\n", md1_noexport());
    return 0;
}
static void __exit md_exit(void)
{
    printk("Module №2 unloaded.\n");
}

module_init(md_init);
module_exit(md_exit);