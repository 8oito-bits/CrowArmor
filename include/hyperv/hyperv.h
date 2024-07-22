#pragma once

#include "crowarmor/crow.h"
#include "hyperv/vmx.h"

struct hyperv_context
{
    int num_online_cpus;
};

bool hyperv_is_vmx_supported(void);
ERR hyperv_init(struct crow **crow);
void hyperv_end(void);