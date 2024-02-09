#include "kpobres/kallsyms_lookup.h"

#include "err/err.h"
#include <linux/kprobes.h>

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