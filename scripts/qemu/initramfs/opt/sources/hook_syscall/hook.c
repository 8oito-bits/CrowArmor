#include <linux/kprobes.h>
#include <linux/module.h>

#include "control_registers/cr0.h"
#include "hook.h"
#include "hook_syscall_table.h"
#include "kpobres/kallsyms_lookup.h"
#include "crowarmor/datacrow.h"

static unsigned long **syscall_table;
static struct crow **armor;

static void
set_new_syscall(struct hook_syscall *syscall)
{
  disable_register_cr0_wp();
  syscall->old_syscall = syscall_table[syscall->idx];
  syscall_table[syscall->idx] = syscall->new_syscall;
  enable_register_cr0_wp();
}

static void
set_old_syscall(struct hook_syscall *syscall)
{
  disable_register_cr0_wp();
  syscall_table[syscall->idx] = syscall->old_syscall;
  enable_register_cr0_wp();
}

void *
hook_get_old_syscall(int idx)
{
  unsigned int i;

  for (i = 0; !IS_NULL_PTR(syscalls[i].new_syscall); i++)
  {
    if (syscalls[i].idx == idx)
      return syscalls[i].old_syscall;
  }

  return NULL;
}

ERR hook_init(struct crow **crow)
{
  unsigned int i;

  syscall_table = (unsigned long **)kallsyms_lookup_name("sys_call_table");

  if (syscall_table == 0)
    return ERR_FAILURE;

  for (i = 0; !IS_NULL_PTR(syscalls[i].new_syscall); i++)
    set_new_syscall(&syscalls[i]);

  (*crow)->hook_is_actived = true;
  armor = crow;

  return ERR_SUCCESS;
}

void hook_end(void)
{
  pr_warn("crowamor: Hook syscalls shutdown ...");

  unsigned int i;

  for (i = 0; !IS_NULL_PTR(syscalls[i].new_syscall); i++)
    set_old_syscall(&syscalls[i]);

  (*armor)->hook_is_actived = false;
}
