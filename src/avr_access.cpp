#include <stdint.h>
#include <string.h>
#include "micro_vm.h"
#include "opcode.h"
#include "debug.h"

#include <avr/pgmspace.h>

#include "code.h"

#include "avr_access.h"

#define MIREP_HEADER_SIZE 4

extern "C" uint32_t read_bytecode(uint8_t irep_id, uint8_t pc){
  uint32_t code = pgm_read_dword( &(mmruby_code_irep_table[irep_id]) + MIREP_HEADER_SIZE + pc );
  return code;
}



