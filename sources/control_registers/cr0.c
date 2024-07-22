#define pr_fmt(fmt) "crowarmor: "fmt

#include "control_registers/cr0.h"

#include <linux/version.h>

#if LINUX_VERSION_CODE >= KERNEL_VERSION(5, 3, 0)

unsigned long get_cr0(void) {
  unsigned long cr0;
  __asm__ volatile("mov    %%cr0, %0": "=r"(cr0));

  return cr0;
}

#else

#define get_cr0 write_cr0

#endif

void set_cr0(unsigned long cr0) {
  __asm__ volatile("mov    %0, %%cr0": : "r"(cr0));
}

void enable_register_cr0_wp(void) {
  unsigned long cr0 = get_cr0();

  cr0 |= 1UL << 16;

  set_cr0(cr0);
}

void disable_register_cr0_wp(void) {
  unsigned long cr0 = get_cr0();

  cr0 &= ~(1UL << 16);

  set_cr0(cr0);
}