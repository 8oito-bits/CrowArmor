#pragma once

#include <linux/types.h>


int __must_check register_driver(void) notrace;
void unregister_driver(void) notrace;
void pr_infos_driver(void) notrace;