#ifndef MRBC_AVR_ACCESS_H_
#define MRBC_AVR_ACCESS_H_

#include <stdint.h>

#ifdef __cplusplus
//extern "C" {
#endif

uint32_t read_bytecode(uint8_t irep_id, uint8_t pc);


#ifdef __cplusplus
//}
#endif
#endif
