#pragma once

#include <linux/types.h>

#ifndef DEBUG
// <...>
#else
bool
verify_this_virtual_machine(void);
#endif