#include "chrdev.h"

#include <linux/atomic.h>
#include <linux/cdev.h>
#include <linux/device.h>
#include <linux/fs.h>
#include <linux/init.h>
#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/platform_device.h>
#include <linux/types.h>
#include <linux/uaccess.h> /* for get_user and put_user */
#include <linux/version.h>

#include "err/err.h"
#include "hook_syscall/hook.h"
#include "io/ioctl.h"

static __always_inline int device_open(struct inode *, struct file *);
static __always_inline int device_release(struct inode *, struct file *);
static __always_inline ssize_t device_read(struct file *, char __user *, size_t,
                                           loff_t *);
static __always_inline ssize_t device_write(struct file *file,
                                            const char __user *buffer,
                                            size_t length, loff_t *offset);
static __always_inline long
device_ioctl(struct file *file,      /* ditto */
             unsigned int ioctl_num, /* number and param for ioctl */
             unsigned long ioctl_param);

static struct class *cls;   /* class for create /dev/<name> */
static struct crow **armor; /* manipulate driver states */

static struct file_operations fops = {
    .read = device_read,
    .write = device_write,
    .open = device_open,
    .release = device_release, /* a.k.a. close */
    .unlocked_ioctl = device_ioctl,
};

static struct platform_driver platform = {
    .driver =
        {
            .name = "crowarmor",
            .owner = THIS_MODULE,
        },
};

int device_open(struct inode *_inode, struct file *_file) {
  int retval = ERR_SUCCESS;

  if (!try_module_get(THIS_MODULE))
    retval = ERR_FAILURE;
  else
    pr_info(
        "crowarmor: Driver has been opened and being used this process PID %d",
        current->pid);

  return retval;
}

/* closes connection in module execute this function */
int device_release(struct inode *_inode, struct file *_file) {

  /* Decrement the usage count, or else once you opened the file, you will
   * never get rid of the module.
   */
  module_put(THIS_MODULE);

  pr_alert("crowarmor: Driver has been closed this process PID %d",
           current->pid);

  return ERR_SUCCESS;
}

ssize_t device_read(struct file *file, char __user *buffer, size_t length,
                    loff_t *offset) {
  if (*offset >= 2) {
    return 0;
  }

  char crowarmor_is_actived[2];
  crowarmor_is_actived[0] = (*armor)->crowarmor_is_actived ? '1' : '0';
  crowarmor_is_actived[1] = '\n';
  int crowarmor_is_actived_len = 2;

  if (copy_to_user(buffer, crowarmor_is_actived, crowarmor_is_actived_len) !=
      0) {
    return -EFAULT;
  }

  *offset += crowarmor_is_actived_len;

  return crowarmor_is_actived_len;
}

/*
The kernel uses our driver indirectly, so to be able to remove our driver
you first need to disable some states of the crowarmor driver, so if the driver
is in use the kernel will prevent it from being removed.

Note: removing the driver directly without disabling the states can
be harmful to the kernel, causing kernel panic in some versions
*/
ssize_t device_write(struct file *file, const char __user *buffer,
                     size_t length, loff_t *offset) {
  char crowarmor_input;

  if (*offset >= 1) {
    pr_warn(
        "crowarmor: Function device_write already executed once, skipping...");
    return -EINVAL;
  }

  if (copy_from_user(&crowarmor_input, buffer, 1) != 0) {
    return -EFAULT;
  }

  if(((*armor)->crowarmor_is_actived && (crowarmor_input >= '1')) || 
      (!(*armor)->crowarmor_is_actived && (crowarmor_input <= '0'))){
    pr_warn(
        "crowarmor: Driver has already been set to %i", crowarmor_input);
    return -EINVAL;
  }

  (*armor)->crowarmor_is_actived = (crowarmor_input >= '1') ? true : false;

  if ((*armor)->crowarmor_is_actived) {
    pr_info("crowarmor: Enabling driver states");
#if LINUX_VERSION_CODE >= KERNEL_VERSION(6, 8, 0)
    
    hook_sys_call_table_x64();
    if (!try_module_get(THIS_MODULE)){
      pr_info("crowarmor: Error in increment references in use kernel module");
      hook_remove_sys_call_table_x64();
      return -EFAULT;
    }

#endif
  } else {
    pr_info("crowarmor: Disabling driver states (Some features may no longer work)");
#if LINUX_VERSION_CODE >= KERNEL_VERSION(6, 8, 0)

    hook_remove_sys_call_table_x64();
    module_put(THIS_MODULE);

#endif
  }

  *offset += 1;

  return length;
}

__always_inline long device_ioctl(struct file *file, unsigned int ioctl_num,
                                  unsigned long ioctl_param) {
  long retval = ERR_SUCCESS;

  switch (ioctl_num) {
  case IOCTL_READ_CROW:
    if (copy_to_user((struct crow *)ioctl_param, &*(*armor),
                     sizeof(*(*armor)))) {
      pr_alert("crowarmor: Error copy to user 'crowarmor struct'\n");
      retval = ERR_FAILURE;
    }
    break;

  case IOCTL_WRITE_CROW_STATES:
    if (copy_to_user((_Bool *)(*armor)->inspector_is_actived,
                     &*(_Bool *)ioctl_param, sizeof(_Bool))) {
      pr_alert("crowarmor: Error write to 'inspector_is_actived'\n");
      retval = ERR_FAILURE;
    }
    break;

  default:
    break;
  }

  return retval;
}

int __must_check chrdev_init(struct crow **crow) {
  int retval = ERR_SUCCESS;

  pr_info("crowarmor: Registering the %s device", platform.driver.name);

  /*
   * create and register a cdev occupying a range of minors
   * ref: https://elixir.bootlin.com/linux/latest/source/fs/char_dev.c#L268
   */
  if (register_chrdev(MAJOR_NUM, platform.driver.name, &fops) == -ENOMEM) {
    pr_alert("crowarmor: Registering char device failed with %d\n", MAJOR_NUM);
    retval = ERR_FAILURE;
    goto _retval;
  }

  pr_info("crowarmor: I was assigned major number %d\n", MAJOR_NUM);

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
  cls = class_create(platform.driver.name);
#else
  cls = class_create(THIS_MODULE, platform.driver.name);
#endif

  /**
   * Verify if class for device_create this ERR_SUCCESS
   */
  if (cls == ERR_PTR(-ENOMEM)) {
    retval = ERR_FAILURE;
    goto _retval;
  }

  device_create(cls, NULL, MKDEV(MAJOR_NUM, 0), NULL, platform.driver.name);
  pr_info("crowarmor: Device created on /dev/%s\n", platform.driver.name);

  (*crow)->chrdev_is_actived = true;
  (*crow)->crowarmor_is_actived = true;

  armor = crow;

_retval:
  return retval;
}

void chrdev_end() {
  pr_alert("crowarmor: Unregistering the %s device", platform.driver.name);
  device_destroy(cls, MKDEV(MAJOR_NUM, 0));
  class_destroy(cls);

  /* Unregister the device */
  unregister_chrdev(MAJOR_NUM, platform.driver.name);
  (*armor)->chrdev_is_actived = false;
}

void pr_infos_driver() {
  pr_info("crowarmor: Module name: %s\n", THIS_MODULE->name);
  pr_info("crowarmor: Module version: %s\n", THIS_MODULE->version);
  pr_info("crowarmor: Module srcversion: %s\n", THIS_MODULE->srcversion);
  pr_info("crowarmor: Module state: %i\n", THIS_MODULE->state);
#if LINUX_VERSION_CODE >= KERNEL_VERSION(6, 4, 0)
  pr_info("crowarmor: Module address: 0x%llx\n",
          (unsigned long long)THIS_MODULE->mem->base);
  pr_info("crowarmor: Module size: 0x%llx\n",
          (unsigned long long)THIS_MODULE->mem->size);
#endif

#ifdef CONFIG_STACKTRACE_BUILD_ID
  pr_info("crowarmor: Module build ID: %p\n", THIS_MODULE->build_id);
#endif
}