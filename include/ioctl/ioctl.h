#pragma once

#include <linux/ioctl.h>

#define MAJOR_NUM 0x101
#define DRIVER_NAME "crowarmor"

#define IOCTL_IOW_ACTIVED _IOW(MAJOR_NUM, 0, int)