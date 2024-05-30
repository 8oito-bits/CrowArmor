#pragma once

#include "crowarmor/datacrow.h"
#include "err/err.h"

const __must_check ERR inspector_init(struct crow **crow) notrace;
void inspector_end(void) notrace;