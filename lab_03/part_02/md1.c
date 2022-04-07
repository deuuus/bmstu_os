#include <linux/init.h>
#include <linux/module.h>
#include <linux/slab.h>
#include "md.h"

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Sirotkina Polina");

char* md1_data = "aaa";
int md1_number = 2022;

extern char *md1_proc(void) 
{
    return md1_data;
}

extern char *md1_local(void) 
{
    return md1_data;
}

extern char *md1_noexport(void) 
{
    return md1_data;
}

EXPORT_SYMBOL(md1_number);
EXPORT_SYMBOL(md1_data);
EXPORT_SYMBOL(md1_proc);

static int __init md_init(void)
{
    printk("Module №1 was loaded.\n");
    return 0;
}

static void __exit md_exit(void)
{
    printk("Module №1 was removed.\n");
}

module_init(md_init);
module_exit(md_exit);