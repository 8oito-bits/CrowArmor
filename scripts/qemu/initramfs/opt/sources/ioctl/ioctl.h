#pragma once

#include <linux/ioctl.h>

#define MAJOR_NUM 0x101
#define DRIVER_NAME "crowarmor"

#define IOCTL_SET_MSG _IOW(MAJOR_NUM, 0, int)