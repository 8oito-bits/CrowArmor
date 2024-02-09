#pragma once

/* Lookup an address.  modname is set to NULL if it's in the kernel. */
__must_check unsigned long
kallsyms_lookup_name(const char *name);