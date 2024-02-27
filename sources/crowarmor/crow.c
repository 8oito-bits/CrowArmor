#include "crowarmor/crow.h"
#include <linux/slab.h>

ERR crow_init(struct crow **crow)
{
    ERR retval = ERR_SUCCESS;

    (*crow) = kmalloc(sizeof(struct crow), __GFP_HIGH);

    (*crow)->chrdev_is_actived = false;
    (*crow)->hook_is_actived = false;
    (*crow)->inspector_is_actived = false;

    return retval;
}

void crow_end(struct crow **crow)
{
    kfree(*crow);
}