#define pr_fmt(fmt) "crowarmor: "fmt

#include "crowarmor/crow.h"
#include <linux/slab.h>

static struct crow **armor; /* manipulate driver states */

ERR crow_init(struct crow **crow) {
  ERR retval = ERR_SUCCESS;

  (*crow) = kmalloc(sizeof(struct crow), __GFP_HIGH);

  (*crow)->crowarmor_is_actived = true;
  (*crow)->chrdev_is_actived = false;
  (*crow)->hook_is_actived = false;
  (*crow)->inspector_is_actived = false;
  (*crow)->hyperv_is_actived = false;

  armor = crow;

  crow_enable_state();
  return retval;
}

void crow_enable_state(void)
{
  (*armor)->crowarmor_is_actived = true;
}

void crow_disable_state(void)
{
  (*armor)->crowarmor_is_actived = false;
}

void crow_end(struct crow **crow) { kfree(*crow); }