#pragma once

#include "err/err.h"
#include "crowarmor/datacrow.h"

__must_check ERR
hook_init(struct crow **crow) notrace;

void hook_end(void) notrace;

__must_check void *
hook_get_old_syscall(int) notrace;
