#pragma once

#include <linux/types.h>
#include "err/err.h"
#include "datacrow.h"

const ERR __must_check crow_init(struct crow **crow) notrace;
void crow_end(struct crow **crow) notrace;