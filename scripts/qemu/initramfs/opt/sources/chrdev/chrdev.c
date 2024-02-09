#include "chrdev.h"

#include <linux/types.h>
#include <linux/atomic.h>
#include <linux/cdev.h>
#include <linux/device.h>
#include <linux/fs.h>
#include <linux/init.h>
#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/uaccess.h> /* for get_user and put_user */
#include <linux/version.h>
#include <linux/atomic.h>

#include "../error/error.h"
#include "../tkp/ioctl/ioctl.h"
#include "../tkp/signals/signals.h"

static __always_inline int device_open(struct inode *, struct file *);
static __always_inline int device_release(struct inode *, struct file *);
static __always_inline ssize_t device_read(struct file *, char __user *, size_t, loff_t *);
static __always_inline ssize_t device_write(struct file *file, const char __user *buffer,
                                            size_t length, loff_t *offset);

static __always_inline long device_ioctl(struct file *file,      /* ditto */
                                         unsigned int ioctl_num, /* number and param for ioctl */
                                         unsigned long ioctl_param);

static struct __device
{
    const char *name;  /* name driver */
    int major;         /* major number assigned to our device driver */
    struct class *cls; /* class for create /dev/<name> */
} device;

static struct file_operations chardev_fops = {
    .read = device_read,
    .write = device_write,
    .open = device_open,
    .release = device_release, /* a.k.a. close */
    .unlocked_ioctl = device_ioctl

};

static atomic_t already_open = ATOMIC_INIT(CDEV_NOT_USED);

int device_open(struct inode *_inode, struct file *_file)
{
    int retval = SUCCESS;

    /**
     *  Get status actualy this module using atomic
     *  If (already_open == CDEV_NOT_USED),
     *  atomically updates already_open to CDEV_EXCLUSIVE_OPEN with acquire ordering.
     */
    if (atomic_cmpxchg(&already_open, CDEV_NOT_USED, CDEV_EXCLUSIVE_OPEN))
    {
        retval = -EBUSY;
    }
    else
    {
        if (!try_module_get(THIS_MODULE))
            retval = ERROR;
        else
            pr_info("crowarmor: Driver has been opened and being used this process PID %d", current->pid);
    }

    return retval;
}

/* closes connection in module execute this function */
int device_release(struct inode *_inode, struct file *_file)
{

    /* We're now ready for our next caller */
    atomic_set(&already_open, CDEV_NOT_USED);

    /* Decrement the usage count, or else once you opened the file, you will
     * never get rid of the module.
     */
    module_put(THIS_MODULE);

    pr_alert("crowarmor: Driver has been closed this process PID %d", current->pid);

    return SUCCESS;
}

ssize_t device_read(struct file *_file, char __user *_user, size_t, loff_t *_loff)
{
    return 0;
}

ssize_t device_write(struct file *file, const char __user *buffer,
                     size_t length, loff_t *offset)
{
    // int size;

    pr_info("crowarmor: device_write(%p,%p,%ld)", file, buffer, length);

    // for (size = 0; size < length && size < 10; size++)
    //     get_user(message[size], buffer + size);

    /* Again, return the number of input characters used. */
    return 0;
}

__always_inline long device_ioctl(struct file *file,
                                  unsigned int ioctl_num,
                                  unsigned long ioctl_param)
{
    long retval = SUCCESS;

    switch (ioctl_num)
    {
    case IOCTL_SET_MSG:
        pr_info("crowarmor: call IOCTL_SET_MSG");
        /* Receive a pointer to a message (in user space) and set that to
         * be the device's message. Get the parameter given to ioctl by
         * the process.
         */
        // struct __kernel_data __user *tmp = (struct __kernel_data __user *)ioctl_param;

        // get_user(ch, tmp);

        // for (int i = 0; ch && i < BUF_LEN; i++, tmp++)
        //     get_user(ch, tmp);

        break;

    default:
        break;
    }

    return retval;
}

int __must_check register_driver(void)
{
    int retval = SUCCESS;
    device.name = DRIVER_NAME;
    device.major = MAJOR_NUM;

    pr_info("crowarmor: Registering the %s device", device.name);

    /*
     * create and register a cdev occupying a range of minors
     * ref: https://elixir.bootlin.com/linux/latest/source/fs/char_dev.c#L268
     */
    register_chrdev(device.major, device.name, &chardev_fops);

    if (device.major < 0)
    {
        pr_alert("crowarmor: Registering char device failed with %d\n", device.major);
        retval = ERROR;
        goto _retval;
    }

    pr_info("crowarmor: I was assigned major number %d\n", device.major);

/**
 * class_create - create a struct class structure
 * @name: pointer to a string for the name of this class.
 *
 * This is used to create a struct class pointer that can then be used
 * in calls to device_create().
 *
 * ref: https://elixir.bootlin.com/linux/latest/source/drivers/base/class.c#L256
 */
#if LINUX_VERSION_CODE >= KERNEL_VERSION(6, 4, 0)
    device.cls = class_create(device.name);
#else
    device.cls = class_create(THIS_MODULE, device.name);
#endif

    /**
     * Verify if class for device_create this success
     */
    if (device.cls != ERR_PTR(-ENOMEM))
    {
        device_create(device.cls, NULL, MKDEV(device.major, 0), NULL, device.name);
        pr_info("crowarmor: Device created on /dev/%s\n", device.name);
    }
    else
        retval = ERROR;

_retval:
    return retval;
}

void unregister_driver(void)
{
    pr_alert("crowarmor: Unregistering the %s device", device.name);
    device_destroy(device.cls, MKDEV(device.major, 0));
    class_destroy(device.cls);

    /* Unregister the device */
    unregister_chrdev(device.major, device.name);
}

void pr_infos_driver()
{
    struct module *mod = THIS_MODULE;

    pr_info("crowarmor: Module name: %s\n", mod->name);
    pr_info("crowarmor: Module version: %s\n", mod->version);
    pr_info("crowarmor: Module srcversion: %s\n", mod->srcversion);
    pr_info("crowarmor: Module state: %i\n", mod->state);
#if LINUX_VERSION_CODE >= KERNEL_VERSION(6, 4, 0)
    pr_info("crowarmor: Module address: 0x%llx\n", (unsigned long long)mod->mem->base);
    pr_info("crowarmor: Module size: 0x%llx\n", (unsigned long long)mod->mem->size);
#endif

#ifdef CONFIG_STACKTRACE_BUILD_ID
    pr_info("crowarmor: Module build ID: %p\n", mod->build_id);
#endif
}