#ifndef MRBC_SRC_MICRO_VM_H_
#define MRBC_SRC_MICRO_VM_H_

#include <stdint.h>
#include "vm_config.h"

#ifndef MMRUBY_TRANSCODE
#include "value.h"
#endif

//Micro Irep
#define MIREP_OFFSET_RLEN 0
#define MIREP_OFFSET_ILEN 1
#define MIREP_OFFSET_PLEN 2
#define MIREP_OFFSET_SLEN 3
#define MIREP_HEADER_SIZE 4 //size of mrb_mirep in Arduino
#define mrb_mirep_size MIREP_HEADER_SIZE 
typedef struct MIREP {
  uint8_t rlen;   // Number of child IREP blocks
  uint8_t ilen;   // Number of bytecode
  uint8_t plen;   // Size of pools field
  uint8_t slen;   // Size of symbols field

  //data order after mirep header
  // 1. code           : length is ilen * CODE_LEN [byte]
  // 2. literal(pools) : length is plen [byte]
  // 3. symbols        : length is slen [byte]
  // 4. ireps          : legnth is rlen [byte]
} mrb_mirep;

//================================================================
/*!@brief
  Call information
*/
typedef struct MCALLINFO {
  mrb_irep_id pc_irep;
  uint16_t  pc;
  mrb_value *current_regs;
  mrb_class *target_class;
  uint8_t   n_args;     // num of args
} mrb_callinfo;


//================================================================
/*!@brief
  Virtual Machine
*/
typedef struct VM {
  mrb_irep_id  irep;
  mrb_irep_id  pc_irep;
  uint16_t pc;

  mrb_value     regs[MAX_REGS_SIZE];
  mrb_value    *current_regs;
  uint16_t      callinfo_top;
  mrb_callinfo callinfo[MAX_CALLINFO_SIZE];

  mrb_class *target_class;

  int8_t flag_preemption;

} mrb_mvm;


inline static void uint16_to_bin( uint16_t x, uint8_t *bin )
{
  bin[0] = (x & 0xFF00) >> 8;
  bin[1] = (x & 0x00FF);
}

#ifndef MRBC_SRC_VM_H_

void init_vm(void);
void run_vm(void);

mrb_mvm* get_vm(void);

//const char *mrbc_get_irep_symbol(const uint8_t *p, int n);
//const char *mrbc_get_callee_name(mrb_mvm *vm);

void mrbc_push_callinfo(mrb_mvm *vm, int n_args);
void mrbc_pop_callinfo(mrb_mvm *vm);

//================================================================
/*!@brief
  Get 32bit value from memory big endian.

  @param  s	Pointer of memory.
  @return	32bit unsigned value.
*/
inline static uint32_t bin_to_uint32( const void *s )
{
  uint32_t x = *((uint32_t *)s);
  return (x << 24) | ((x & 0xff00) << 8) | ((x >> 8) & 0xff00) | (x >> 24);
}

//================================================================
/*!@brief
  Get 16bit value from memory big endian.

  @param  s	Pointer of memory.
  @return	16bit unsigned value.
*/
inline static uint16_t bin_to_uint16( const void *s )
{
  uint16_t x = *((uint16_t *)s);
  return (x << 8) | (x >> 8);
}

#endif //MRBC_SRC_VM_H_

#endif //MRBC_SRC_MICRO_VM_H_
