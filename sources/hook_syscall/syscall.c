#include <linux/cred.h>
#include <linux/uidgid.h>

#include "hook_syscall/hook.h"
#include "syscall.h"

#ifdef HOOK_SYSCALL_TABLE

struct wsyscall {
  int nr_syscall;
  const struct pt_regs *regs;
  pid_t pid;
};

asmlinkage long syscall_memfd_create(const struct pt_regs *regs) {
  pid_t pid;
  struct wsyscall syscall;

  asmlinkage long (*syscall_old_memfd)(const struct pt_regs *) =
      hook_get_old_syscall(__NR_memfd_create);

  syscall.pid = pid = task_pid_nr(current);
  syscall.nr_syscall = __NR_memfd_create;
  syscall.regs = regs;

  pr_info("crowarmor: Process %i called memfd_create syscall\n", pid);

  return syscall_old_memfd(regs);
}
#endif
