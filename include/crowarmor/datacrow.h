#pragma once

#include <linux/types.h>

struct crow {
  bool crowarmor_is_actived;
  bool chrdev_is_actived;
  bool hook_is_actived;
  bool inspector_is_actived;
  bool hyperv_is_actived;
};