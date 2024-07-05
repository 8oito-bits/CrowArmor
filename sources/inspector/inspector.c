#include "inspector/inspector.h"

#include "control_registers/cr0.h"
#include "control_registers/cr4.h"
#include "err/err.h"
#include "hook_syscall/hook.h"

#include <linux/delay.h>
#include <linux/kernel.h>
#include <linux/kthread.h>
#include <linux/sched.h>
#include <linux/unistd.h>

inline static bool check_bit_cr0wp(void);
inline static bool check_bit_cr4pvi(void);
inline static int inspector_thread_function(void *data);

static void check_hooked_syscalls(void);

static struct task_struct *inspector_thread;
struct crow **armor;

ERR inspector_init(struct crow **crow) {
  ERR retval = ERR_SUCCESS;

  inspector_thread = kthread_run(inspector_thread_function, NULL,
                                 "crowarmor/inspector/thread");
  if (!inspector_thread)
    retval = ERR_FAILURE;

  pr_info("crowarmor: Thread inspector running ...");

  (*crow)->inspector_is_actived = true;
  armor = crow;
  return retval;
}

void inspector_end(void) {
  kthread_stop(inspector_thread);
  (*armor)->inspector_is_actived = false;
  pr_warn("crowarmor: Thread inspector shutdown ...");
}

static void check_hooked_syscalls(void) {
  struct hook_syscall syscall;
  size_t i;
  for (i = 0; i < __NR_syscalls; i++) {
    hook_check_hooked_syscall(&syscall, i);
    if (syscall.unknown_hook) {
      pr_info("crowarmor: syscall %i hooked by %lx\n", syscall.idx,
              (unsigned long)syscall.new_syscall);
      pr_info("crowarmor: restoring syscall...");
      hook_remove_unknown_syscall(&syscall);
      pr_info("crowarmor: syscall restored");
    }
  }
}

bool check_bit_cr0wp(void) { return (get_cr0() >> 16) & 0x1; }

bool check_bit_cr4pvi(void) { return (get_cr4() >> 1) & 0x1; }

int inspector_thread_function(void *data) {
  while (!kthread_should_stop()) {
    if (!check_bit_cr0wp()) {
      pr_alert("crowarmor: CR0 write-protect bit not set (0); setting "
               "write-protect bit to 1");
      check_hooked_syscalls();
      enable_register_cr0_wp();
    }
    /*
     * When protected-mode virtual interrupts are disabled (that is, when the
     * PVI flag in control register CR4 is set to 0, the CPL is less than 3, or
     * the IOPL value is 3), then the CLI and STI instructions execute in a
     * manner compatible with the Intel486 processor. That is, if the CPL is
     * greater (less privileged) than the I/O privilege level (IOPL), a
     * general-protection exception occurs. If the IOPL value is 3, CLI and STI
     * clear or set the IF flag, respectively.
     * PUSHF, POPF, IRET and INT are executed like in the Intel486 processor,
     * regardless of whether protected-mode virtual interrupts are enabled. It
     * is only possible to enter virtual-8086 mode through a task switch or the
     * execution of an IRET instruction, and it is only possible to leave
     * virtual-8086 mode by faulting Vol. 3A 15-31 8086 EMULATION to a
     * protected-mode interrupt handler (typically the general-protection
     * exception handler, which in turn calls the virtual 8086-mode monitor). In
     * both cases, the EFLAGS register is saved and restored. This is not true,
     * however, in protected mode when the PVI flag is set and the processor is
     * not in virtual-8086 mode. Here, it is possible to call a procedure at a
     * different privilege level, in which case the EFLAGS register is not saved
     * or modified. However, the states of VIF and VIP flags are never examined
     * by the processor when the CPL is not 3
     */
    if (check_bit_cr4pvi()) {
      pr_alert("crowramor: CR4 rotected-mode-virtual-interrupts bit to set "
               "(1); setting rotected-mode-virtual-interrupts bit to 0");
      disable_register_cr4_pvi();
    }

    msleep(5);
  }

  return ERR_SUCCESS;
}
