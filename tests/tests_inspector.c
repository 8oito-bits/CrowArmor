#include <linux/kernel.h> /* We are doing kernel work */
#include <linux/module.h> /* Specifically, a module  */
#include <linux/kprobes.h>

static void *old_syscall;
static void **syscall_table;

unsigned long
get_cr0(void)
{
  unsigned long cr0;
  __asm__ volatile("mov    %%cr0, %0" : "=r"(cr0));

  return cr0;
}

void
set_cr0(unsigned long cr0)
{
  __asm__ volatile("mov    %0, %%cr0" : : "r"(cr0));
}

void
enable_register_cr0_wp(void)
{
  unsigned long cr0 = get_cr0();

  cr0 |= 1UL << 16;

  set_cr0(cr0);
}

void
disable_register_cr0_wp(void)
{
  unsigned long cr0 = get_cr0();

  cr0 &= ~(1UL << 16);

  set_cr0(cr0);
}

unsigned long
kallsyms_lookup_name(const char *name)
{
  unsigned long (*kallsyms_lookup_name)(const char *name);

  struct kprobe kp = {
          .symbol_name = "kallsyms_lookup_name",
  };

  if (register_kprobe(&kp) < 0) return 0;

  kallsyms_lookup_name = (unsigned long (*)(const char *name))kp.addr;

  unregister_kprobe(&kp);

  return (unsigned long)kallsyms_lookup_name(name);
}

asmlinkage long
syscall_memfd_create(const struct pt_regs *regs)
{
        asmlinkage long (*syscall_old_memfd)(const struct pt_regs *) =
          old_syscall;

        pr_info("tests_inspector: hooked function \n");

        return syscall_old_memfd(regs);
}

int __init
init_module(void)
{
  disable_register_cr0_wp();
  syscall_table = (void **) kallsyms_lookup_name("sys_call_table");
  old_syscall = syscall_table[__NR_memfd_create];
  syscall_table[__NR_memfd_create] = syscall_memfd_create;
  pr_info("tests_inspector: Starting driver for tests \n");
  return 0;
}

void __exit
cleanup_module(void)
{
  syscall_table[__NR_memfd_create] = old_syscall;
  pr_warn("tests_inspector: Shutdown driver for tests \n");
  enable_register_cr0_wp();
}

MODULE_AUTHOR("MalDec Labs");
MODULE_DESCRIPTION("Driver to tests");
MODULE_LICENSE("GPL");
MODULE_VERSION("1.0.0");
