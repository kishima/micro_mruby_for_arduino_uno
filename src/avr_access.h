#ifndef MRBC_AVR_ACCESS_H_
#define MRBC_AVR_ACCESS_H_

#include <stdint.h>
#include "value.h"

//access irep
uint32_t read_bytecode(mrb_irep_id irep_id, uint8_t pc);
void get_irep_pool(uint8_t* buff, uint16_t* obj_size, mrb_irep_id irep_id, uint8_t no);
mrb_irep_id get_irep_irep_id(mrb_irep_id irep_id,uint8_t no);
mrb_sym get_irep_symbol_id(mrb_irep_id irep_id, uint8_t no);
uint8_t get_max_static_symbol_id();

//for symbol
mrb_sym search_index_static(const char* str);

//for class
mrb_proc *find_static_procs(mrb_vtype tt, mrb_sym sym_id);

#endif
