#include <linux/kernel.h> /* We are doing kernel work */
#include <linux/module.h> /* Specifically, a module  */

#include "chrdev/chrdev.h"
#include "crowarmor/crow.h"
#include "err/err.h"
#include "hook_syscall/hook.h"
#include "inspector/inspector.h"

static struct crow *crow;

int __init init_module(void) {
  pr_info("crowamor: Starting driver crowarmor \n");

  ERR retval = crow_init(&crow);

  if (!IS_ERR_FAILURE(retval)) {
    if (!IS_ERR_FAILURE(chrdev_init(&crow))) {
      retval = ERR_SUCCESS;
      pr_infos_driver();
      /*...*/
    }

    if (!IS_ERR_FAILURE(retval) && !IS_ERR_FAILURE(hook_init(&crow))) {
      retval = ERR_SUCCESS;

      /*...*/
    }

    if (!IS_ERR_FAILURE(retval) && !IS_ERR_FAILURE(inspector_init(&crow))) {
      retval = ERR_SUCCESS;

      /*...*/
    }
  }

  return retval;
}

void __exit cleanup_module(void) {
  pr_warn("crowamor: Shutdown driver crowarmor\n");
  hook_end();
  chrdev_end();
  inspector_end();
  crow_end(&crow);
}

MODULE_AUTHOR("MalDec Labs");
MODULE_DESCRIPTION("Driver to hardening");
MODULE_LICENSE("GPL");
MODULE_VERSION("1.0.0");
