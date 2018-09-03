#include "vm_config.h"
#include <string.h>

#include "value.h"
#include "micro_vm.h"
#include "opcode.h"
#include "class.h"
#include "alloc.h"
#include "c_string.h"
#include "c_fixnum.h"
#include "global.h"
#include "symbol.h"
#include "console.h"
#include "debug.h"
#include "avr_access.h"
#include "symbol_ids.h"


//================================================================
/*! (operator) [] bit reference
 */
void c_fixnum_bitref(mrb_mvm *vm, mrb_value v[], int argc)
{
  if( 0 <= v[1].i && v[1].i < 32 ) {
    SET_INT_RETURN( (v[0].i & (1 << v[1].i)) ? 1 : 0 );
  } else {
    SET_INT_RETURN( 0 );
  }
}


//================================================================
/*! (operator) unary -
*/
void c_fixnum_negative(mrb_mvm *vm, mrb_value v[], int argc)
{
  int32_t num = GET_INT_ARG(0);
  SET_INT_RETURN( -num );
}


//================================================================
/*! (operator) ** power
 */
void c_fixnum_power(mrb_mvm *vm, mrb_value v[], int argc)
{
  if( v[1].tt == MRB_TT_FIXNUM ) {
    int32_t x = 1;
    int i;

    if( v[1].i < 0 ) x = 0;
    for( i = 0; i < v[1].i; i++ ) {
      x *= v[0].i;;
    }
    SET_INT_RETURN( x );
  }

}


//================================================================
/*! (operator) %
 */
void c_fixnum_mod(mrb_mvm *vm, mrb_value v[], int argc)
{
  int32_t num = GET_INT_ARG(1);
  SET_INT_RETURN( v->i % num );
}


//================================================================
/*! (operator) &; bit operation AND
 */
void c_fixnum_and(mrb_mvm *vm, mrb_value v[], int argc)
{
  int32_t num = GET_INT_ARG(1);
  SET_INT_RETURN(v->i & num);
}


//================================================================
/*! (operator) |; bit operation OR
 */
void c_fixnum_or(mrb_mvm *vm, mrb_value v[], int argc)
{
  int32_t num = GET_INT_ARG(1);
  SET_INT_RETURN(v->i | num);
}


//================================================================
/*! (operator) ^; bit operation XOR
 */
void c_fixnum_xor(mrb_mvm *vm, mrb_value v[], int argc)
{
  int32_t num = GET_INT_ARG(1);
  SET_INT_RETURN( v->i ^ num );
}


//================================================================
/*! (operator) ~; bit operation NOT
 */
void c_fixnum_not(mrb_mvm *vm, mrb_value v[], int argc)
{
  int32_t num = GET_INT_ARG(0);
  SET_INT_RETURN( ~num );
}


//================================================================
/*! x-bit left shift for x
 */
static int32_t shift(int32_t x, int32_t y)
{
  if( y >= 33 ){
    x = 0;
  } else if( y >= 0 ){
    x <<= y;
  } else if( y > -33 ){
    x = x >> -y;
  } else {
    x = 0;
  }
  return x;
}


//================================================================
/*! (operator) <<; bit operation LEFT_SHIFT
 */
void c_fixnum_lshift(mrb_mvm *vm, mrb_value v[], int argc)
{
  int num = GET_INT_ARG(1);
  SET_INT_RETURN( shift(v->i, num) );
}


//================================================================
/*! (operator) >>; bit operation RIGHT_SHIFT
 */
void c_fixnum_rshift(mrb_mvm *vm, mrb_value v[], int argc)
{
  int num = GET_INT_ARG(1);
  SET_INT_RETURN( shift(v->i, -num) );
}


//================================================================
/*! (method) abs
*/
void c_fixnum_abs(mrb_mvm *vm, mrb_value v[], int argc)
{
  if( v[0].i < 0 ) {
    v[0].i = -v[0].i;
  }
}


//================================================================
/*! (method) times
*/
void c_fixnum_times(mrb_mvm *vm, mrb_value v[], int argc)
{
  //TODO
  #if 0
  uint32_t code[2] = {
    MKOPCODE(OP_CALL) | MKARG_A(argc),
    MKOPCODE(OP_ABORT)
  };
  mrb_mirep irep = {
    0,     // nlocals
    0,     // nregs
    0,     // rlen
    2,     // ilen
    0,     // plen
    (uint8_t *)code,   // iseq
    NULL,  // pools
    NULL,  // ptr_to_sym
    NULL,  // reps
  };

  // count of times
  int cnt = v[0].i;

  mrb_value block = v[1];
  mrbc_dup(&block);

  mrbc_push_callinfo(vm, 0);

  // adjust reg_top for reg[0]==Proc
  vm->current_regs += v - vm->regs + 1;

  int i;
  for( i=0 ; i<cnt ; i++ ){
    if(v[1].tt!=MRB_TT_PROC){//TODO:in normal case, ref is increased. Need to be checked
      v[1]=block;
      if(block.proc->ref_count < 2) mrbc_dup(&block);
    }
    // set index
    mrbc_release( &v[2] );
    v[2].tt = MRB_TT_FIXNUM;
    v[2].i = i;

    // set OP_CALL irep
    vm->pc = 0;
    vm->pc_irep = &irep;

    // execute OP_CALL
    mrbc_vm_run(vm);
  }
  mrbc_pop_callinfo(vm);
  #endif
}



//================================================================
/*! (method) chr
*/
void c_fixnum_chr(mrb_mvm *vm, mrb_value v[], int argc)
{
  //TODO
  #if 0
  char buf[2] = { GET_INT_ARG(0) };

  mrb_value value = mrbc_string_new(vm, buf, 1);
  SET_RETURN(value);
  #endif
}


//================================================================
/*! (method) to_s
*/
void c_fixnum_to_s(mrb_mvm *vm, mrb_value v[], int argc)
{
  int base = 10;
  if( argc ) {
    base = GET_INT_ARG(1);
    if( base < 2 || base > 36 ) {
      return;	// raise ? ArgumentError
    }
  }

  mrb_printf pf;
  char buf[16];
  //sprintf(buf,"%d",v->i);
  mrbc_printf_init( &pf, buf, sizeof(buf), NULL );
  pf.fmt.type = 'd';
  mrbc_printf_int( &pf, v->i, base );
  mrbc_printf_end( &pf );

  mrb_value value = mrbc_string_new_cstr(buf);
  SET_RETURN(value);
}


void mrbc_init_class_fixnum()
{
    mrbc_class_fixnum = mrbc_define_class((char*)MRBC_SSYM_Fixnum, mrbc_class_object);
}
