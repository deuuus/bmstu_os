#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/vmalloc.h>
#include <linux/proc_fs.h>
#include <linux/uaccess.h>

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Sirotkina Polina");

#define MAX_BUF_SIZE PAGE_SIZE

#define DIRNAME "fortune_dir"
#define FILENAME "fortune"
#define SYMLINK "fortune_ln"
#define FILEPATH DIRNAME "/" FILENAME

static struct proc_dir_entry *fortune_dir = NULL;
static struct proc_dir_entry *fortune = NULL;
static struct proc_dir_entry *fortune_ln = NULL;

static char *cookie_buffer;
static int next_index;
static int current_fortune;

static char tmp[MAX_BUF_SIZE];

ssize_t fortune_read(struct file *filp, char __user *buf, size_t count, loff_t *offp)
{
    int len;

    printk("+: read called\n");

    if (*offp > 0 || !next_index)
    {
        printk("+: offp case");
        return 0;
    }

    if (current_fortune >= next_index)
    {
        current_fortune = 0;
    }

    len = snprintf(tmp, MAX_BUF_SIZE, "%s\n", &cookie_buffer[current_fortune]);
    if (copy_to_user(buf, tmp, len))
    {
        printk(KERN_ERR "+: error copy_to_user\n");
        return -EFAULT;
    }

    current_fortune += len;
    *offp += len;

    return len;
}

ssize_t fortune_write(struct file *filp, const char __user *buf, size_t len, loff_t *offp)
{
    printk("+: write called\n");
    
    if (len > MAX_BUF_SIZE - next_index + 1)
    {
        printk(KERN_ERR "+: err cookie_buffer overflow\n");
        return -ENOSPC;
    }

    if (copy_from_user(&cookie_buffer[next_index], buf, len))
    {
        printk(KERN_ERR "+: error copy_from_user\n");
        return -EFAULT;
    }

    next_index += len;
    cookie_buffer[next_index - 1] = '\0';

    return len;
}

int fortune_open(struct inode *inode, struct file *file)
{
    printk(KERN_INFO "+: called open.\n");
    return 0;
}

int fortune_release(struct inode *inode, struct file *file)
{
    printk(KERN_INFO "+: called release.\n");
    return 0;
}

static struct proc_ops fops = 
{
    .proc_read = fortune_read,
    .proc_write = fortune_write,
    .proc_open = fortune_open,
    .proc_release = fortune_release
};

static void freemem(void)
{
    if (fortune_ln)
    {
        remove_proc_entry(SYMLINK, NULL);
    }

    if (fortune)
    {
        remove_proc_entry(FILENAME, fortune_dir);
    }

    if (fortune_dir)
    {
        remove_proc_entry(DIRNAME, NULL);
    }

    if (cookie_buffer)
    {
        vfree(cookie_buffer);
    }
}

static int __init fortune_init(void)
{
    if (!(cookie_buffer = vmalloc(MAX_BUF_SIZE)))
    {
        freemem();
        printk(KERN_ERR "+: error vmalloc\n");
        return -ENOMEM;
    }

    memset(cookie_buffer, 0, MAX_BUF_SIZE);

    if (!(fortune_dir = proc_mkdir(DIRNAME, NULL)))
    {
        freemem();
        printk(KERN_ERR "+: error proc_mkdir\n");
        return -ENOMEM;
    }
    else if (!(fortune = proc_create(FILENAME, 0666, fortune_dir, &fops)))
    {
        freemem();
        printk(KERN_ERR "+: error proc_create\n");
        return -ENOMEM;
    }
    else if (!(fortune_ln = proc_symlink(SYMLINK, NULL, FILEPATH)))
    {
        freemem();
        printk(KERN_ERR "+: error proc_symlink\n");
        return -ENOMEM;
    }

    next_index = 0;
    current_fortune = 0;

    printk(KERN_INFO "+: module loaded.\n");

    return 0;
}

static void __exit fortune_exit(void)
{
    freemem();
    printk(KERN_INFO "+: module unloaded.\n");
}

module_init(fortune_init)
module_exit(fortune_exit)