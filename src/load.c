#if 0

#include <stdint.h>
#include <string.h>
#include "vm.h"
#include "opcode.h"
#include "load.h"
#include "debug.h"

#include <avr/pgmspace.h>

static mrb_irep current_irep;

//List of static ireps
// bytecode shall be preanalyzed
uint16_t PROGMEM irep_head_pointer_list={
  16,
  23,
};

inline initial_load_static_irep(uint8_t id, uint8_t *pos){
  
}

inline find_static_irep(uint8_t id, uint8_t *pos){
  //pickup all ireps from FROM in Arduino
  
}

inline load_static_irep(uint8_t id, uint8_t *pos){
  uint8_t *p = *pos + 4; // skip record size

  // new irep
  mrb_irep *irep = &current_irep;//mrbc_irep_alloc(0);

  // nlocals,nregs,rlen
  irep->nlocals = pgm_read_word(p); p += 2;
  irep->nregs   = pgm_read_word(p); p += 2;
  irep->rlen    = pgm_read_word(p); p += 2;
  irep->ilen    = pgm_read_dword(p);p += 4;

  // padding
  //p += (vm->mrb - p) & 0x03;

  // allocate memory for child irep's pointers
  if( irep->rlen ) {
    irep->reps = (mrb_irep **)mrbc_alloc(0, sizeof(mrb_irep *) * irep->rlen);
    if( irep->reps == NULL ) {
      vm->error_code = LOAD_FILE_IREP_ERROR_ALLOCATION;
      return NULL;
    }
  }

  // ISEQ (code) BLOCK
  irep->code = (uint8_t *)p;
  p += irep->ilen * 4;

  // POOL BLOCK
  irep->plen = bin_to_uint32(p);	p += 4;
  if( irep->plen ) {
    irep->pools = (mrb_object**)mrbc_alloc(0, sizeof(void*) * irep->plen);
    if(irep->pools == NULL ) {
      vm->error_code = LOAD_FILE_IREP_ERROR_ALLOCATION;
      return NULL;
    }
  }

  int i;
  for( i = 0; i < irep->plen; i++ ) {
    int tt = *p++;
    int obj_size = bin_to_uint16(p);	p += 2;
    mrb_object *obj = mrbc_obj_alloc(0, MRB_TT_EMPTY);
    if( obj == NULL ) {
      vm->error_code = LOAD_FILE_IREP_ERROR_ALLOCATION;
      return NULL;
    }
    switch( tt ) {
#if MRBC_USE_STRING
    case 0: { // IREP_TT_STRING
      obj->tt = MRB_TT_STRING;
      obj->str = (char*)p;
    } break;
#endif
    case 1: { // IREP_TT_FIXNUM
      char buf[obj_size+1];
      memcpy(buf, p, obj_size);
      buf[obj_size] = '\0';
      obj->tt = MRB_TT_FIXNUM;
      obj->i = atol(buf);
    } break;
#if MRBC_USE_FLOAT
    case 2: { // IREP_TT_FLOAT
      char buf[obj_size+1];
      memcpy(buf, p, obj_size);
      buf[obj_size] = '\0';
      obj->tt = MRB_TT_FLOAT;
      obj->d = atof(buf);
    } break;
#endif
    default:
      break;
    }

    irep->pools[i] = obj;
    p += obj_size;
  }

  // SYMS BLOCK
  irep->ptr_to_sym = (uint8_t*)p;
  int slen = bin_to_uint32(p);		p += 4;
  while( --slen >= 0 ) {
    int s = bin_to_uint16(p);		p += 2;
    p += s+1;
  }

  *pos = p;
  return irep;

}

mrb_irep* load_mirep(mrb_mirep mirep){
  if(id & MIREP_MASK){
    return (mrb_irep*)(mirep->head);
  }
  return load_irep(id);
}

#endif
