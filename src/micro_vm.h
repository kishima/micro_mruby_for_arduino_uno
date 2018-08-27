#ifndef MRBC_SRC_MICRO_VM_H_
#define MRBC_SRC_MICRO_VM_H_

#include <stdint.h>
#include "vm_config.h"

#ifndef MMRUBY_TRANSCODE
#include "value.h"
#endif

#ifdef __cplusplus
extern "C" {
#endif

//Micro Irep
typedef struct MIREP {
  //uint8_t nlocals;// # of local variables
  //uint8_t nregs;  // # of register variables
  uint8_t rlen;   // # of child IREP blocks
  uint8_t ilen;   // # of iSeq
  uint8_t plen;   // # of pool
  uint8_t slen;   // # of symbols

  //uint8_t irep_list[]; // irep_id list
  //uint8_t     *code; // ISEQ (code) BLOCK
                       // NOT Needed. Can be define by offset using sizeof(mrb_mirep)
  //mrb_object  **pools; // array of POOL objects pointer.
                         // NOT Needed. Can be define by offset using ilen
  
} mrb_mirep;


//size of mrb_mirep in Arduino
#define mrb_mirep_size  7

// get irep 
uint8_t* get_irep_p(uint8_t irep_id);
     
//irep_id : irep ID
/*
#define code_ptr(irep_id) (get_irep_p(irep_id) + mrb_mirep_size)
#define pools_ptr(irep_id) (get_irep_p(irep_id) + mrb_mirep_size \
                                + progmem_read_byte(get_irep_p(irep_id)+2) \ //rlen
                                + progmem_read_byte(get_irep_p(irep_id)+3) ) //ilen
#define sym_ptr(irep_id) (get_irep_p(irep_id) + mrb_mirep_size \
                                + progmem_read_byte(get_irep_p(irep_id)+2) \ //rlen
                                + progmem_read_word(get_irep_p(irep_id)+5) ) //sym_pos
*/
//================================================================
/*!@brief
  Call information
*/
typedef struct MCALLINFO {
  mrb_mirep *pc_irep;
  uint16_t  pc;
  mrb_value *current_regs;
  mrb_class *target_class;
  uint8_t   n_args;     // num of args
} mrb_mcallinfo;


//================================================================
/*!@brief
  Virtual Machine
*/
typedef struct VM {
  mrb_mirep *irep;

  mrb_mirep *pc_irep;
  uint16_t  pc;

  mrb_value    regs[MAX_REGS_SIZE];
  mrb_value   *current_regs;
  uint16_t     callinfo_top;
  mrb_mcallinfo callinfo[MAX_CALLINFO_SIZE];

  mrb_class *target_class;

  int8_t flag_preemption;

} mrb_mvm;


inline static void uint16_to_bin( uint16_t x, uint8_t *bin )
{
  bin[0] = (x & 0xFF00) >> 8;
  bin[1] = (x & 0x00FF);
}

#ifndef MRBC_SRC_VM_H_

const char *mrbc_get_irep_symbol(const uint8_t *p, int n);
const char *mrbc_get_callee_name(mrb_mvm *vm);
//mrb_vm *mrbc_vm_open(mrb_vm *vm_arg);
//void mrbc_vm_close(mrb_vm *vm);
//void mrbc_vm_begin(mrb_vm *vm);
//void mrbc_vm_end(mrb_vm *vm);
int mrbc_vm_run(mrb_mvm *vm);

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

#ifdef __cplusplus
}
#endif
#endif
