#pragma once

/**
 * CR0
 * | Bit | Label | Description           |
 * | --- | ----- | --------------------- |
 * | 0   | PE    | Protected Mode Enable |
 * | 1   | MP    | Monitor co-processor  |
 * | 2   | EM    | x87 FPU Emulation     |
 * | 3   | TS    | Task switched         |
 * | 4   | ET    | Extension type        |
 * | 5   | NE    | Numeric error         |
 * | 16  | WP    | Write protect         |
 * | 18  | AM    | Alignment mask        |
 * | 29  | NW    | Not-write through     |
 * | 30  | CD    | Cache disable         |
 * | 31  | PG    | Paging                |
 */

const unsigned long get_cr0(void);
void set_cr0(unsigned long cr0);

/**
 *   | Bit | Label | Description           |
 *   | --- | ----- | --------------------- |
 *   | 16  | WP    | Write protect         |
 */
void enable_register_cr0_wp(void);
void disable_register_cr0_wp(void);