#ifndef MRBC_AVR_ACCESS_H_
#define MRBC_AVR_ACCESS_H_

#include <stdint.h>

//access irep
uint32_t read_bytecode(uint8_t irep_id, uint8_t pc);
void get_irep_pool(uint8_t* buff, uint16_t* obj_size, uint8_t irep_id, uint8_t no);
uint8_t get_irep_symbol_id(uint8_t* buff, uint16_t* sym_size, uint8_t irep_id, uint8_t no);

#endif
