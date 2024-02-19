#pragma once

#include <linux/ioctl.h>

#define MAJOR_NUM 0x101

struct crowarmor
{
    _Bool is_actived;
};

#define IOCTL_READ_CROWARMOR _IOR(MAJOR_NUM, 0, struct crowarmor *)