#include <linux/cred.h>
#include <linux/uidgid.h>

#include "hook.h"
#include "syscall.h"

asmlinkage long
syscall_memfd_create(const struct pt_regs *regs)
{
  int pid;

  asmlinkage long (*syscall_old_memfd)(const struct pt_regs *) =
          hook_get_old_syscall(__NR_memfd_create);

  pid = task_pid_nr(current);
  pr_info("crowamor: Process %i blocked due to memfd_create system call\n",
          pid);

  return syscall_old_memfd(regs);
}
