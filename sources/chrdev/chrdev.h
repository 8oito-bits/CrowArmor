#pragma once

#include <linux/types.h>
#include "crowarmor/datacrow.h"

const __must_check int chrdev_init(struct crow **crow) notrace;
void chrdev_end(void) notrace;
void pr_infos_driver(void) notrace;