/* 
 * Entry Point 
 */
ENTRY(Reset_Handler)

/* 
 * Highest address of the user mode stack 
 */
_estack = 0x20004000;                                                           /* end of 16K RAM */

/* 
 * Generate a link error if heap and stack don't fit into RAM 
 */
_Min_Heap_Size = 0;                                                             /* required amount of heap  */
_Min_Stack_Size = 0x80;                                                         /* required amount of stack */

/* 
 * Specify the memory areas 
 */
MEMORY
{
  FLASH (rx)      : ORIGIN = 0x08000000, LENGTH = 128K
  RAM (xrw)       : ORIGIN = 0x20000000, LENGTH = 16K
  MEMORY_B1 (rx)  : ORIGIN = 0x60000000, LENGTH = 0K
  RW_EEPROM (rw)   : ORIGIN = 0x08080000, LENGTH = 32
}

INCLUDE "../../port/arch/ARM_CM3/hal/script/arm-cm3_rom.ld"
