#pragma once

#include "err/err.h"
#include "crowarmor/datacrow.h"

const __must_check ERR
inspector_init(struct crow **crow) notrace;
void inspector_end(void) notrace;