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


void mrbc_init_class_object(){
  DEBUG_FPRINTLN("-- define Object class");
  // Class
  mrbc_class_object = mrbc_define_class("Object", 0);
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
