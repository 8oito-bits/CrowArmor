#pragma once

#include "err/err.h"

__must_check ERR
hook_init(void);
void
hook_end(void);
__must_check void *
hook_get_old_syscall(int);
