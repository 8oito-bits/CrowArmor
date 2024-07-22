#pragma once

#include "crowarmor/crow.h"

bool hyperv_is_vmx_supported(void);
ERR hyperv_init(struct crow **crow);
void hyperv_end(void);