#include "control_registers/cr4.h"

unsigned long
get_cr4(void)
{
  unsigned long cr4;
  __asm__ volatile("mov    %%cr4, %0" : "=r"(cr4));

  return cr4;
}

void
set_cr4(unsigned long cr4)
{
  __asm__ volatile("mov    %0, %%cr4" : : "r"(cr4));
}

void
enable_register_cr4_pvi(void)
{
  unsigned long cr4 = get_cr4();

  cr4 |= 1UL << 1;

  set_cr4(cr4);
}

void
disable_register_cr4_pvi(void)
{
  unsigned long cr4 = get_cr4();

  cr4 &= ~(1UL << 1);

  set_cr4(cr4);
}
