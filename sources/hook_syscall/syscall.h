#pragma once

#include <linux/module.h>

/**
 * @brief asmlinkage - asmlinkage is a clause used to ensure that system call functions maintain a consistent and reliable 
 *                     interface between user space and kernel space, ensuring that necessary resources are accessible on the stack.
 *
 * @return asmlinkage
 */
asmlinkage long
syscall_memfd_create(const struct pt_regs *);
