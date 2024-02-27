#pragma once

/**
 *  | Bit | Label      | Description                                                       |
 *  |-----|------------|-------------------------------------------------------------------|
 *  | 0   | VME        | Virtual 8086 Mode Extensions                                      |
 *  | 1   | PVI        | Protected-mode Virtual Interrupts                                 |
 *  | 2   | TSD        | Time Stamp Disable                                                |
 *  | 3   | DE         | Debugging Extensions                                              |
 *  | 4   | PSE        | Page Size Extension                                               |
 *  | 5   | PAE        | Physical Address Extension                                        |
 *  | 6   | MCE        | Machine Check Exception                                           |
 *  | 7   | PGE        | Page Global Enabled                                               |
 *  | 8   | PCE        | Performance-Monitoring Counter enable                             |
 *  | 9   | OSFXSR     | Operating system support for FXSAVE and FXRSTOR instructions      |
 *  | 10  | OSXMMEXCPT | Operating System Support for Unmasked SIMD Floating-Point Exceptions|
 *  | 11  | UMIP       | User-Mode Instruction Prevention (if set, #GP on SGDT, SIDT, SLDT, SMSW, and STR instructions when CPL > 0)|
 *  | 13  | VMXE       | Virtual Machine Extensions Enable                                 |
 *  | 14  | SMXE       | Safer Mode Extensions Enable                                      |
 *  | 16  | FSGSBASE   | Enables the instructions RDFSBASE, RDGSBASE, WRFSBASE, and WRGSBASE|
 *  | 17  | PCIDE      | PCID Enable                                                        |
 *  | 18  | OSXSAVE    | XSAVE and Processor Extended States Enable                         |
 *  | 20  | SMEP       | Supervisor Mode Execution Protection Enable                        |
 *  | 21  | SMAP       | Supervisor Mode Access Prevention Enable                           |
 *  | 22  | PKE        | Protection Key Enable                                              |
 *  | 23  | CET        | Control-flow Enforcement Technology                                |
 *  | 24  | PKS        | Enable Protection Keys for Supervisor-Mode Pages                   |
 */

const unsigned long
get_cr4(void);
void
set_cr4(unsigned long cr4);

/**
 * | Bit | Label | Description           |
 * | --- | ----- | --------------------- |
 * |  1  | PVI    |  Protected-mode Virtual Interrupts  |
 */
void
enable_register_cr4_pvi(void);
void
disable_register_cr4_pvi(void);
