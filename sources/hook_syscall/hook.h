#pragma once

#include "err/err.h"
#include "crowarmor/datacrow.h"

struct hook_syscall
{
  void *new_syscall;
  void *old_syscall;
  bool crowarmor_hook;
  bool unknown_hook;
  int   idx;
};

__must_check ERR
hook_init(struct crow **crow) notrace;

void hook_end(void) notrace;

__must_check void *
hook_get_old_syscall(int) notrace;

void hook_check_hooked_syscall(struct hook_syscall *syscall, int idx);
