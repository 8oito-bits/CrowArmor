#pragma once

#include "err/err.h"

__must_check ERR
inspector_registers_controls_init(void) notrace;
void inspector_registers_controls_end(void) notrace;