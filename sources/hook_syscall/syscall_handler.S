#include <linux/linkage.h>

.code64

.extern hook_syscall_handler

.section .text
.global crowarmor_syscall_handler

SYM_FUNC_START(crowarmor_syscall_handler)
#  callq  hook_syscall_handler_c
  jmp *syscall_handler(%rip)
SYM_FUNC_END(crowarmor_syscall_handler)
