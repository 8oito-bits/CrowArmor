#include "hyperv/hyperv.h"

#include <linux/printk.h>
#include <linux/types.h>

static struct crow **armor;

ERR hyperv_init(struct crow **crow)
{
	armor = crow;

	if (!hyperv_is_vmx_supported())
	{
		pr_warn("crowarmor: hyperv has not been initialized");
		return ERR_FAILURE;
	}

	pr_info("crowarmor: VMX supported in CPU, hyperv initializing ...");

	(*armor)->hyperv_is_actived = true;

	return ERR_SUCCESS;
}

void hyperv_end(void)
{
	(*armor)->hyperv_is_actived = false;
}