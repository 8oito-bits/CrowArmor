#pragma once

#include <linux/ioctl.h>
#include "crowarmor/datacrow.h"

#define MAJOR_NUM 0x101

#define IOCTL_READ_CROW _IOR(MAJOR_NUM, 0, struct crow *)
