#pragma once

#include "err/err.h"

__must_check ERR
hook_init(void) notrace;

void hook_end(void) notrace;

__must_check void *
hook_get_old_syscall(int) notrace;
