#pragma once

#include "syscall.h"

struct hook_syscall
{
  void *new_syscall;
  void *old_syscall;
  int   idx;
};

struct hook_syscall syscalls[] = {
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