#pragma once

#include <linux/types.h>

int __must_check register_driver(void);
void unregister_driver(void);
void pr_infos_driver(void);