#pragma once

#include "err/err.h"
#include <asm/signal.h>
#include <linux/sched.h>
#include <linux/sched/signal.h>

ERR
send_signal_connection(void);