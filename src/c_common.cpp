#include "vm_config.h"
#include <string.h>

#include "value.h"
#include "micro_vm.h"
#include "class.h"
#include "alloc.h"
#include "c_string.h"
#include "global.h"
#include "symbol.h"
#include "console.h"
#include "debug.h"
#include "avr_access.h"
#include "symbol_ids.h"

/* Proc class */
void c_proc_call(mrb_mvm *vm, mrb_value v[], int argc)
{
  // push callinfo, but not release regs
  mrbc_push_callinfo(vm, argc);

  // target irep
  vm->pc = 0;
  vm->pc_irep = v[0].proc->irep;

  vm->current_regs = v;
}

void c_proc_to_s(mrb_mvm *vm, mrb_value v[], int argc)
{
  // (NOTE) address part assumes 32bit. but enough for this.
  char buf[32];
  mrb_printf pf;

  mrbc_printf_init( &pf, buf, sizeof(buf), "<#Proc:%08x>" );
  while( mrbc_printf_main( &pf ) > 0 ) {
    mrbc_printf_int( &pf, (uintptr_t)v->proc, 16 );
  }
  mrbc_printf_end( &pf );

  SET_RETURN( mrbc_string_new_cstr( buf ) );
}

/* Nil class */
void c_nil_to_s(mrb_mvm *vm, mrb_value v[], int argc)
{
  v[0] = mrbc_string_new( NULL, 0);
}

/* False class */
void c_false_to_s(mrb_mvm *vm, mrb_value v[], int argc)
{
  v[0] = mrbc_string_new_cstr( "false");
}

/* True class */
void c_true_to_s(mrb_mvm *vm, mrb_value v[], int argc)
{
  v[0] = mrbc_string_new_cstr( "true");
}


void mrbc_init_class_proc(void){
  mrbc_class_proc = mrbc_define_class((char*)MRBC_SSYM_Proc, 0);
}

void mrbc_init_class_nil(void){
  mrbc_class_nil = mrbc_define_class((char*)MRBC_SSYM_Nil, 0);
}

void mrbc_init_class_false(void){
  mrbc_class_false = mrbc_define_class((char*)MRBC_SSYM_False, 0);
}

void mrbc_init_class_true(void){
  mrbc_class_true = mrbc_define_class((char*)MRBC_SSYM_True, 0);
  //methods
  // - to_s
}

