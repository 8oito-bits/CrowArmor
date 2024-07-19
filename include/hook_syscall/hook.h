#pragma once

#include "crowarmor/datacrow.h"
#include "err/err.h"
#include <linux/kprobes.h>
#include <linux/version.h>

#if LINUX_VERSION_CODE < KERNEL_VERSION(6, 8, 0)
#define HOOK_SYSCALL_TABLE
#endif

const __must_check ERR hook_init(struct crow **crow) notrace;
void hook_end(void) notrace;

#ifdef HOOK_SYSCALL_TABLE
struct hook_syscall {
  void *new_syscall;
  void *old_syscall;
  bool unknown_hook;
  int idx;
};

const __must_check void *hook_get_old_syscall(unsigned int) notrace;
void hook_remove_unknown_syscall(struct hook_syscall *) notrace;
void hook_check_hooked_syscall(struct hook_syscall *, unsigned int) notrace;
#else
void hook_syscall_handler_c(void);
#endif
