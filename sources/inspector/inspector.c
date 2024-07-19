#include "inspector/inspector.h"

#include "control_registers/cr0.h"
#include "control_registers/cr4.h"
#include "err/err.h"
#include "hook_syscall/hook.h"
#include "hyperv.h"

#include <linux/delay.h>
#include <linux/kernel.h>
#include <linux/kthread.h>
#include <linux/sched.h>
#include <linux/unistd.h>

struct crow **armor;

ERR inspector_init(struct crow **crow)
{
  ERR retval = ERR_SUCCESS;

  pr_info("crowarmor: Inspector running ...");

  (*crow)->inspector_is_actived = true;
  armor = crow;

  return retval;
}

void inspector_end(void)
{
  (*armor)->inspector_is_actived = false;
  pr_warn("crowarmor: Inspector shutdown ...");
}

#ifdef HOOK_SYSCALL_TABLE

static void check_hooked_syscalls(void);

static void check_hooked_syscalls(void)
{
  struct hook_syscall syscall;
  size_t i;
  for (i = 0; i < __NR_syscalls; i++)
  {
    hook_check_hooked_syscall(&syscall, i);
    if (syscall.unknown_hook)
    {
      pr_info("crowarmor: syscall %i hooked by %lx\n", syscall.idx,
              (unsigned long)syscall.new_syscall);
      pr_info("crowarmor: restoring syscall...");
      hook_remove_unknown_syscall(&syscall);
      pr_info("crowarmor: syscall restored");
    }
  }
}
#endif

ERR inspector_run(void)
{
  if (hyperv_is_vmx_supported())
  {
#ifdef HOOK_SYSCALL_TABLE
    check_hooked_syscalls();
#endif
  }
  return ERR_SUCCESS;
}
