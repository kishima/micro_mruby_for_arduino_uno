#ifndef __MMRUBY_VM_H_
#define __MMRUBY_VM_H_

#if 0

#include <stdint.h>
#include "vm_config.h"
#include "value.h"

#ifdef __cplusplus
extern "C" {
#endif

#define MIREP_MASK (0x80)

typedef struct MICRO_IREP {
  uint8_t  id;   //If (id&0x80) this is a dynamic
  uint8_t* head; //Head pointer of the irep 
} mrb_mirep; 


typedef struct IREP {
  uint16_t nlocals;		//!< # of local variables
  uint16_t nregs;		//!< # of register variables
  uint16_t rlen;		//!< # of child IREP blocks
  uint16_t ilen;		//!< # of irep
  uint16_t plen;		//!< # of pool

  uint8_t     *code;		//!< ISEQ (code) BLOCK
  mrb_object  **pools;          //!< array of POOL objects pointer.
  uint8_t     *ptr_to_sym;
  mrb_mirep   *reps;		//!< array of child IREP's pointer.

} mrb_irep;



typedef struct CALLINFO {
  mrb_irep_ds pc_irep;
  uint16_t  pc;
  mrb_value *current_regs;
  mrb_class *target_class;
  uint8_t   n_args;     // num of args
} mrb_callinfo;


typedef struct VM {
  mrb_irep_ds irep; //?

  //uint8_t        vm_id; // vm_id : 1..n  //REMOVE
  //const uint8_t *mrb;   // bytecode //REMOVE?

  mrb_irep_ds pc_irep;    // PC  //Dynamic
  uint16_t    pc;         // PC  //Dynamic

  mrb_value    regs[MAX_REGS_SIZE]; //Dynamic
  mrb_value   *current_regs; //Dynamic
  uint16_t     callinfo_top; //Dynamic
  mrb_callinfo callinfo[MAX_CALLINFO_SIZE]; //Dynamic

  mrb_class *target_class; //Dynamic

  //int32_t error_code; //REMOVE

  //volatile int8_t flag_preemption; //REMOVE?
  //int8_t flag_need_memfree; //REMOVE?
} mrb_vm;

//---------------------------------------

inline static uint32_t bin_to_uint32( const void *s )
{
  uint32_t x = *((uint32_t *)s);
  return (x << 24) | ((x & 0xff00) << 8) | ((x >> 8) & 0xff00) | (x >> 24);
}

inline static uint16_t bin_to_uint16( const void *s )
{
  uint16_t x = *((uint16_t *)s);
  return (x << 8) | (x >> 8);
}
inline static void uint16_to_bin(uint16_t s, uint8_t *bin)
{
  *bin++ = (s >> 8) & 0xff;
  *bin   = s & 0xff;
}
static inline void uint32_to_bin(uint32_t l, uint8_t *bin)
{
  *bin++ = (l >> 24) & 0xff;
  *bin++ = (l >> 16) & 0xff;
  *bin++ = (l >> 8) & 0xff;
  *bin   = l & 0xff;
}

void init_vm(void);
void run_vm(void);

#ifdef __cplusplus
}
#endif


#endif // comment out

#endif
