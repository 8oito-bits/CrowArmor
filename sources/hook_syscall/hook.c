#include <linux/kprobes.h>
#include <linux/module.h>
#include <linux/string.h>
#include "control_registers/cr0.h"
#include "hook.h"
#include "kpobres/kallsyms_lookup.h"
#include "crowarmor/datacrow.h"
#include "syscall.h"

static unsigned long **syscall_table;
static unsigned long **old_syscall_table;
static struct crow **armor;

static struct hook_syscall syscalls[] = {
        /**
         * hooked system calls
         */
        {.new_syscall = syscall_memfd_create,
         .old_syscall = NULL,
         .idx         = __NR_memfd_create},

        /**
         * null byte array
         */
        {.new_syscall = NULL, .old_syscall = NULL, .idx = -1}

};

static int
get_syscalls_idx(int idx)
{
  switch(idx)
  {
    case __NR_memfd_create:
      return 0;
    default:
      return -1;
  }
}

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

void
hook_check_hooked_syscall(struct hook_syscall *syscall, int idx)
{
  if(syscall_table[idx] != old_syscall_table[idx])
  {
    syscall->new_syscall = syscall_table[idx];
    syscall->old_syscall = old_syscall_table[idx];
    syscall->idx = idx;
    if(get_syscalls_idx(idx) == -1)
    {
      syscall->crowarmor_hook = false;
      syscall->unknown_hook = true;
    }
    else
    {
      syscall->crowarmor_hook = true;
      syscall->unknown_hook = false;
    } 
  }
}

ERR hook_init(struct crow **crow)
{
  unsigned int i;

  syscall_table = (unsigned long **)kallsyms_lookup_name("sys_call_table");

  if (syscall_table == 0)
    return ERR_FAILURE;
  
  old_syscall_table = kmalloc(sizeof(void *) * __NR_syscalls, __GFP_HIGH);

  if(syscall_table == NULL)
    return ERR_FAILURE;

  memcpy(old_syscall_table, syscall_table, sizeof(void *) * __NR_syscalls);

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
