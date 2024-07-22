#define pr_fmt(fmt) "crowarmor: "fmt

#include "hook_syscall/hook.h"
#include "control_registers/cr0.h"
#include "crowarmor/datacrow.h"
#include "kprobes/kallsyms_lookup.h"
#include "syscall.h"
#include "syscall_handler.h"

#include <linux/kprobes.h>
#include <linux/module.h>
#include <linux/nospec.h>
#include <linux/string.h>
#include <asm/msr.h>
#include <asm/msr-index.h>

static struct crow **armor;

#ifdef HOOK_SYSCALL_TABLE
static unsigned long **old_syscall_table;
static unsigned long **syscall_table;
static unsigned long **crowarmor_syscall_table;

static struct hook_syscall syscalls[] = {
    /**
     * hooked system calls
     */
    {.new_syscall = syscall_memfd_create,
     .old_syscall = NULL,
     .idx = __NR_memfd_create},

    /**
     * null byte array
     */
    {.new_syscall = NULL, .old_syscall = NULL, .idx = -1}

};

static void set_new_syscall(struct hook_syscall *syscall) {
  disable_register_cr0_wp();
  syscall->old_syscall = syscall_table[syscall->idx];
  syscall_table[syscall->idx] = syscall->new_syscall;
  enable_register_cr0_wp();
}

static void set_old_syscall(struct hook_syscall *syscall) {
  disable_register_cr0_wp();
  syscall_table[syscall->idx] = syscall->old_syscall;
  enable_register_cr0_wp();
}

void hook_remove_unknown_syscall(struct hook_syscall *syscall) {
  disable_register_cr0_wp();
  syscall_table[syscall->idx] = syscall->old_syscall;
  enable_register_cr0_wp();
}

const void *hook_get_old_syscall(unsigned int idx) { return old_syscall_table[idx]; }

void hook_check_hooked_syscall(struct hook_syscall *syscall, unsigned int idx) {
  syscall->unknown_hook = false;

  if (syscall_table[idx] != crowarmor_syscall_table[idx]) {
    syscall->new_syscall = syscall_table[idx];
    syscall->old_syscall = crowarmor_syscall_table[idx];
    syscall->idx = idx;

    syscall->unknown_hook = true;
  }
}
#else
void *syscall_handler;

static void setup_lstar_hook(void *syscall_handler)
{
  wrmsrl(MSR_LSTAR, (unsigned long) syscall_handler);
}

void hook_syscall_handler_c(void)
{
  pr_info("Process %i called memfd_create syscall\n", task_pid_nr(current));
}
#endif

ERR hook_init(struct crow **crow) {
  
#ifdef HOOK_SYSCALL_TABLE
  unsigned int i;

  syscall_table = (unsigned long **)kallsyms_lookup_name("sys_call_table");

  if (syscall_table == NULL) {
    pr_info("Failed to get syscall table\n");
    return ERR_FAILURE;
  }

  old_syscall_table = kmalloc(sizeof(void *) * __NR_syscalls, __GFP_HIGH);

  if (old_syscall_table == NULL)
    return ERR_FAILURE;

  crowarmor_syscall_table = kmalloc(sizeof(void *) * __NR_syscalls, __GFP_HIGH);

  if (crowarmor_syscall_table == NULL)
    return ERR_FAILURE;

  memcpy(old_syscall_table, syscall_table, sizeof(void *) * __NR_syscalls);

  for (i = 0; !IS_NULL_PTR(syscalls[i].new_syscall); i++)
    set_new_syscall(&syscalls[i]);

  memcpy(crowarmor_syscall_table, syscall_table,
         sizeof(void *) * __NR_syscalls);
#else
  syscall_handler = (void *) __rdmsr(MSR_LSTAR);
  on_each_cpu(setup_lstar_hook, crowarmor_syscall_handler, 1);
#endif

  (*crow)->hook_is_actived = true;
  armor = crow;

  return ERR_SUCCESS;
}

void hook_end(void) {
  pr_warn("Hook syscalls shutdown ...\n");

#ifdef HOOK_SYSCALL_TABLE
  for (unsigned int i = 0; !IS_NULL_PTR(syscalls[i].new_syscall); i++)
    set_old_syscall(&syscalls[i]);

  kfree(old_syscall_table);
  kfree(crowarmor_syscall_table);

  (*armor)->hook_is_actived = false;
#else
  on_each_cpu(setup_lstar_hook, syscall_handler, 1);
#endif
}
