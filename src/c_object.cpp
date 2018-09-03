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

/* Object Class */

#ifdef MRBC_DEBUG
void mrbc_p_sub(mrb_value *v)
{
  switch( v->tt ){
  case MRB_TT_EMPTY:	DEBUG_FPRINTLN("(empty)");	break;
  case MRB_TT_NIL:	DEBUG_FPRINTLN("nil");		break;

  case MRB_TT_FALSE:
  case MRB_TT_TRUE:
  case MRB_TT_FIXNUM:
  case MRB_TT_FLOAT:
  case MRB_TT_CLASS:
  case MRB_TT_OBJECT:
  case MRB_TT_PROC:
    mrbc_puts_sub(v);
    break;

  case MRB_TT_SYMBOL:{
    const char *s = mrbc_symbol_cstr( v );
    char *fmt = strchr(s, ':') ? "\":%s\"" : ":%s";
    console_printf(fmt, s);
  } break;

  case MRB_TT_ARRAY:{
    console_putchar('[');
    int i;
    for( i = 0; i < mrbc_array_size(v); i++ ) {
      if( i != 0 ) console_print(", ");
      mrb_value v1 = mrbc_array_get(v, i);
      mrbc_p_sub(&v1);
    }
    console_putchar(']');
  } break;

  case MRB_TT_STRING:{
    console_putchar('"');
    const char *s = mrbc_string_cstr(v);
    int i;
    for( i = 0; i < mrbc_string_size(v); i++ ) {
      if( s[i] < ' ' || 0x7f <= s[i] ) {	// tiny isprint()
        console_printf("\\x%02x", s[i]);
      } else {
        console_putchar(s[i]);
      }
    }
    console_putchar('"');
  } break;

  case MRB_TT_RANGE:{
    mrb_value v1 = mrbc_range_first(v);
    mrbc_p_sub(&v1);
    console_print( mrbc_range_exclude_end(v) ? "..." : ".." );
    v1 = mrbc_range_last(v);
    mrbc_p_sub(&v1);
  } break;

  case MRB_TT_HASH:{
    console_putchar('{');
    mrb_hash_iterator ite = mrbc_hash_iterator(v);
    while( mrbc_hash_i_has_next(&ite) ) {
      mrb_value *vk = mrbc_hash_i_next(&ite);
      mrbc_p_sub(vk);
      console_print("=>");
      mrbc_p_sub(vk+1);
      if( mrbc_hash_i_has_next(&ite) ) console_print(", ");
    }
    console_putchar('}');
  } break;

  default:
    console_printf("MRB_TT_XX(%d)", v->tt);
    break;
  }
}
#endif

int mrbc_puts_sub(mrb_value *v)
{
  int ret = 0;

  switch( v->tt ){
/*
  case MRB_TT_NIL:					break;
  case MRB_TT_FALSE:	console_print("false");		break;
  case MRB_TT_TRUE:	console_print("true");		break;
  case MRB_TT_FIXNUM:	console_printf("%d", v->i);	break;
  //case MRB_TT_FLOAT:    console_printf("%g", v->d);	break;
  case MRB_TT_SYMBOL:
    console_print( mrbc_symbol_cstr( v ) );
    break;

  case MRB_TT_CLASS:
    console_print( symid_to_str( v->cls->sym_id ) );
    break;

  case MRB_TT_OBJECT:
    console_printf( "#<%s:%08x>",
	symid_to_str( find_class_by_object(0,v)->sym_id ), v->instance );
    break;

  case MRB_TT_PROC:
    console_print( "#<Proc>" );
    break;

  case MRB_TT_ARRAY:{
    int i;
    for( i = 0; i < mrbc_array_size(v); i++ ) {
      if( i != 0 ) console_putchar('\n');
      mrb_value v1 = mrbc_array_get(v, i);
      mrbc_puts_sub(&v1);
    }
  } break;
*/
  case MRB_TT_STRING: {
    const char *s = mrbc_string_cstr(v);
    console_print(s);
    if( strlen(s) != 0 && s[strlen(s)-1] == '\n' ) ret = 1;
  } break;

  /*
  case MRB_TT_RANGE:{
    mrb_value v1 = mrbc_range_first(v);
    mrbc_puts_sub(&v1);
    console_print( mrbc_range_exclude_end(v) ? "..." : ".." );
    v1 = mrbc_range_last(v);
    mrbc_puts_sub(&v1);
  } break;

  case MRB_TT_HASH:
#ifdef MRBC_DEBUG
    mrbc_p_sub(v);
#else
    console_print("#<Hash>");
#endif
    break;
*/
  default:
    console_printf("MRB_TT_XX(%d)", v->tt);
    break;
  }

  return ret;
}

void c_puts(mrb_mvm *vm, mrb_value v[], int argc)
{
  int i;
  for( i = 1; i <= argc; i++ ) {
    if( mrbc_puts_sub( &v[i] ) == 0 ) console_putchar('\n');
  }
}


void c_object_not(mrb_mvm *vm, mrb_value v[], int argc)
{
  SET_FALSE_RETURN();
}

// Object !=
void c_object_neq(mrb_mvm *vm, mrb_value v[], int argc)
{
  int result = mrbc_compare(v, v+1);

  if( result ) {
    SET_TRUE_RETURN();
  } else {
    SET_FALSE_RETURN();
  }
}


//================================================================
/*! (operator) <=>
 */
void c_object_compare(mrb_mvm *vm, mrb_value v[], int argc)
{
  int result = mrbc_compare( &v[0], &v[1] );

  SET_INT_RETURN(result);
}


// Object#class
void c_object_class(mrb_mvm *vm, mrb_value v[], int argc)
{
  mrb_class *cls = find_class_by_object( v );
  mrb_value value = mrbc_string_new_cstr(symid_to_str(cls->sym_id) );
  SET_RETURN(value);
}

// Object.new
void c_object_new(mrb_mvm *vm, mrb_value v[], int argc)
{
  //TODO
  #if 0
  mrb_value new_obj = mrbc_instance_new(vm, v->cls, 0);

  char syms[]="______initialize";
  mrb_sym sym_id = str_to_symid(&syms[6]);
  mrb_proc *m = find_method(vm, v[0], sym_id);
  if( m==0 ){
    SET_RETURN(new_obj);
    return;
  }
  uint32_to_bin( 1,(uint8_t*)&syms[0]);
  uint16_to_bin(10,(uint8_t*)&syms[4]);

  uint32_t code[2] = {
    MKOPCODE(OP_SEND) | MKARG_A(0) | MKARG_B(0) | MKARG_C(argc),
    MKOPCODE(OP_ABORT)
    };
   mrb_irep irep = {
    0,     // nlocals
    0,     // nregs
    0,     // rlen
    2,     // ilen
    0,     // plen
    (uint8_t *)code,   // iseq
    NULL,  // pools
    (uint8_t *)syms,  // ptr_to_sym
    NULL,  // reps
  };

  mrbc_release(&v[0]);
  v[0] = new_obj;
  mrbc_dup(&new_obj);

  mrb_irep *org_pc_irep = vm->pc_irep;
  uint16_t  org_pc = vm->pc;
  mrb_value* org_regs = vm->current_regs;
  vm->pc = 0;
  vm->pc_irep = &irep;
  vm->current_regs = v;

  mrbc_vm_run(vm);

  vm->pc = org_pc;
  vm->pc_irep = org_pc_irep;
  vm->current_regs = org_regs;

  SET_RETURN(new_obj);
  #endif
}

//================================================================
/*! (method) instance variable getter
 */
void c_object_getiv(mrb_mvm *vm, mrb_value v[], int argc)
{
  //TODO
  #if 0
  const char *name = mrbc_get_callee_name(vm);
  mrb_sym sym_id = str_to_symid( name );
  mrb_value ret = mrbc_instance_getiv(&v[0], sym_id);

  SET_RETURN(ret);
  #endif
}


//================================================================
/*! (method) instance variable setter
 */
void c_object_setiv(mrb_mvm *vm, mrb_value v[], int argc)
{
  //TODO
  #if 0
  const char *name = mrbc_get_callee_name(vm);

  char *namebuf = mrbc_alloc(vm, strlen(name));
  if( !namebuf ) return;
  strcpy(namebuf, name);
  namebuf[strlen(name)-1] = '\0';	// delete '='
  mrb_sym sym_id = str_to_symid(namebuf);

  mrbc_instance_setiv(&v[0], sym_id, &v[1]);
  mrbc_raw_free(namebuf);
  #endif
}



//================================================================
/*! (class method) access method 'attr_reader'
 */
void c_object_attr_reader(mrb_mvm *vm, mrb_value v[], int argc)
{
  int i;
  for( i = 1; i <= argc; i++ ) {
    if( v[i].tt != MRB_TT_SYMBOL ) continue;	// TypeError raise?

    // define reader method
    const char *name = mrbc_symbol_cstr(&v[i]);
    mrbc_define_method(v[0].cls, name, c_object_getiv);
  }
}


//================================================================
/*! (class method) access method 'attr_accessor'
 */
void c_object_attr_accessor(mrb_mvm *vm, mrb_value v[], int argc)
{
  int i;
  for( i = 1; i <= argc; i++ ) {
    if( v[i].tt != MRB_TT_SYMBOL ) continue;	// TypeError raise?

    // define reader method
    const char *name = mrbc_symbol_cstr(&v[i]);
    mrbc_define_method(v[0].cls, name, c_object_getiv);

    // make string "....=" and define writer method.
    char *namebuf = (char*)mrbc_alloc(vm, strlen(name)+2);
    if( !namebuf ) return;
    strcpy(namebuf, name);
    strcat(namebuf, "=");
    mrbc_symbol_new(vm, namebuf);
    mrbc_define_method(v[0].cls, namebuf, c_object_setiv);
    mrbc_raw_free(namebuf);
  }
}

//================================================================
/*! (method) to_s
 */
void c_object_to_s(mrb_mvm *vm, mrb_value v[], int argc)
{
  // (NOTE) address part assumes 32bit. but enough for this.

  char buf[32];
  mrb_printf pf;

  mrbc_printf_init( &pf, buf, sizeof(buf), "#<%s:%08x>" );
  while( mrbc_printf_main( &pf ) > 0 ) {
    switch(pf.fmt.type) {
    case 's':
      mrbc_printf_str( &pf, symid_to_str(v->instance->cls->sym_id), ' ' );
      break;
    case 'x':
      mrbc_printf_int( &pf, (uintptr_t)v->instance, 16 );
      break;
    }
  }
  mrbc_printf_end( &pf );

  SET_RETURN( mrbc_string_new_cstr( buf ) );
}

#ifdef MRBC_DEBUG
static void c_object_instance_methods(mrb_mvm *vm, mrb_value v[], int argc)
{
  // TODO: check argument.

  // temporary code for operation check.
  console_printf( "[" );
  int flag_first = 1;

  mrb_class *cls = find_class_by_object( vm, v );
  mrb_proc *proc = cls->procs;
  while( proc ) {
    console_printf( "%s:%s", (flag_first ? "" : ", "),
		    symid_to_str(proc->sym_id) );
    flag_first = 0;
    proc = proc->next;
  }

  console_printf( "]" );

  SET_NIL_RETURN();
}
#endif


void mrbc_init_class_object(){
  // Class
  mrbc_class_object = mrbc_define_class((char*)MRBC_SSYM_Object, 0);
  // Methods
  // These are defined statically
#if 0
  mrbc_define_method(mrbc_class_object, "puts", c_puts);
  mrbc_define_method(mrbc_class_object, "!", c_object_not);
  mrbc_define_method(mrbc_class_object, "!=", c_object_neq);
  mrbc_define_method(mrbc_class_object, "<=>", c_object_compare);
  mrbc_define_method(mrbc_class_object, "to_s", c_object_to_s);
  mrbc_define_method(mrbc_class_object, "new", c_object_new);
  mrbc_define_method(mrbc_class_object, "class", c_object_class);
  mrbc_define_method(mrbc_class_object, "attr_reader", c_object_attr_reader);
  mrbc_define_method(mrbc_class_object, "attr_accessor", c_object_attr_accessor);
#endif
  
}
