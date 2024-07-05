#pragma once

#include "datacrow.h"
#include "err/err.h"
#include <linux/types.h>

const ERR __must_check crow_init(struct crow **crow) notrace;
void crow_end(struct crow **crow) notrace;
void crow_enable_state(void) notrace;
void crow_disable_state(void) notrace;