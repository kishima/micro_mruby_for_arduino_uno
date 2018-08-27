#include <stdint.h>
#include <string.h>
#include "micro_vm.h"
#include "opcode.h"
#include "console.h"
#include "debug.h"

#include <avr/pgmspace.h>

#include "code.h"

#include "avr_access.h"

#define MIREP_HEADER_SIZE 4

extern "C" uint32_t read_bytecode(uint8_t irep_id, uint8_t pc){
  //uint32_t t = pgm_read_dword( &(mmruby_code_irep_table[irep_id]) + MIREP_HEADER_SIZE + pc );
  uint8_t t1 = pgm_read_byte( &(mmruby_code_irep_table[irep_id]) + MIREP_HEADER_SIZE + pc );
  uint8_t t2 = pgm_read_byte( &(mmruby_code_irep_table[irep_id]) + MIREP_HEADER_SIZE + pc + 1);
  uint8_t t3 = pgm_read_byte( &(mmruby_code_irep_table[irep_id]) + MIREP_HEADER_SIZE + pc + 2);
  uint8_t t4 = pgm_read_byte( &(mmruby_code_irep_table[irep_id]) + MIREP_HEADER_SIZE + pc + 3);
  //return (uint32_t)(t1<<24 | t2<<16 | t3<<8 | t4);
  console_printf("%02x %02x %02x %02x \n",t4,t3,t2,t1);
  return (uint32_t)(t4<<24 | t3<<16 | t2<<8 | t1);
}



