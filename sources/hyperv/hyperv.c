#define pr_fmt(fmt) "crowarmor: "fmt

#include "hyperv/hyperv.h"

#include <linux/mm.h>
#include <linux/printk.h>
#include <linux/types.h>

static struct crow **armor;
static struct hyperv_context hyperv;

ERR hyperv_init(struct crow **crow)
{
	armor = crow;

	if (!hyperv_is_vmx_supported())
	{
		pr_warn("hyperv has not been initialized\n");
		return ERR_FAILURE;
	}

	pr_info("VMX supported in CPU, hyperv initializing ...\n");

	hyperv.num_online_cpus = num_online_cpus();

	(*armor)->hyperv_is_actived = true;

	return ERR_SUCCESS;
}

void hyperv_end(void)
{
	(*armor)->hyperv_is_actived = false;
}