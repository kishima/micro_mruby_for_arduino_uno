#include <stdint.h>
#include <string.h>
#include "micro_vm.h"
#include "opcode.h"
#include "console.h"
#include "debug.h"

#include <Arduino.h>
#include <avr/pgmspace.h>

#include "code.h"

#include "avr_access.h"

#define MIREP_HEADER_SIZE 4

extern "C" uint32_t read_bytecode(uint8_t irep_id, uint8_t pc){
  return pgm_read_dword_near( pgm_read_word_near(&mmruby_code_irep_table[irep_id]) + MIREP_HEADER_SIZE + pc*CODE_LEN );
}



