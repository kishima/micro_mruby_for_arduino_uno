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

/* Class Tree */
mrb_class *mrbc_class_object;
mrb_class *mrbc_class_proc;
mrb_class *mrbc_class_false;
mrb_class *mrbc_class_true;
mrb_class *mrbc_class_nil;
mrb_class *mrbc_class_array;
mrb_class *mrbc_class_fixnum;
mrb_class *mrbc_class_symbol;
mrb_class *mrbc_class_string;
mrb_class *mrbc_class_range;
//mrb_class *mrbc_class_hash;

/* Common */

void c_ineffect(mrb_mvm *vm, mrb_value v[], int argc)
{
  // nothing to do.
}


/* Object Class */

#ifdef MRBC_DEBUG
void mrbc_p_sub(mrb_value *v)
{
  switch( v->tt ){
  case MRB_TT_EMPTY:	console_print("(empty)");	break;
  case MRB_TT_NIL:	console_print("nil");		break;

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

static void c_puts(mrb_mvm *vm, mrb_value v[], int argc)
{
  int i;
  for( i = 1; i <= argc; i++ ) {
    if( mrbc_puts_sub( &v[i] ) == 0 ) console_putchar('\n');
  }
}


/* Util Function */
mrb_class *find_class_by_object(struct VM *vm, mrb_object *obj)
{
  mrb_class *cls;

  switch( obj->tt ) {
  case MRB_TT_TRUE:	cls = mrbc_class_true;		break;
  case MRB_TT_FALSE:	cls = mrbc_class_false; 	break;
  case MRB_TT_NIL:	cls = mrbc_class_nil;		break;
  case MRB_TT_FIXNUM:	cls = mrbc_class_fixnum;	break;
  //case MRB_TT_FLOAT:	cls = mrbc_class_float; 	break;
  case MRB_TT_SYMBOL:	cls = mrbc_class_symbol;	break;

  case MRB_TT_OBJECT:	cls = obj->instance->cls;       break;
  case MRB_TT_CLASS:    cls = obj->cls;                 break;
  case MRB_TT_PROC:	cls = mrbc_class_proc;		break;
  case MRB_TT_ARRAY:	cls = mrbc_class_array; 	break;
  case MRB_TT_STRING:	cls = mrbc_class_string;	break;
  case MRB_TT_RANGE:	cls = mrbc_class_range; 	break;
  //TODO
  //case MRB_TT_HASH:	cls = mrbc_class_hash;		break;

  default:		cls = mrbc_class_object;	break;
  }

  return cls;
}

mrb_proc *find_method(mrb_mvm *vm, mrb_value recv, mrb_sym sym_id)
{
  mrb_class *cls = find_class_by_object(vm, &recv);

  while( cls != 0 ) {
    mrb_proc *proc = cls->procs;
    while( proc != 0 ) {
      if( proc->sym_id == sym_id ) {
        return proc;
      }
      proc = proc->next;
    }
    cls = cls->super;
  }
  return 0;
}

mrb_class * mrbc_define_class(mrb_mvm* vm, const char *name, mrb_class *super)
{
  DEBUG_FPRINTLN("define class");
  mrb_class *cls;
  mrb_sym sym_id = str_to_symid(name);
  mrb_object obj = const_object_get(sym_id);

  // create a new class?
  if( obj.tt == MRB_TT_NIL ) {
    cls = (mrb_class*)mrbc_alloc( 0, sizeof(mrb_class) );
    if( !cls ) return cls;	// ENOMEM

    cls->sym_id = sym_id;
    cls->super = super;
    cls->procs = 0;

    // register to global constant.
    mrb_value v = {.tt = MRB_TT_CLASS};
    v.cls = cls;
    const_object_add(sym_id, &v);

    return cls;
  }

  // already?
  if( obj.tt == MRB_TT_CLASS ) {
    return obj.cls;
  }
}


void mrbc_define_method(mrb_mvm *vm, mrb_class *cls, const char *name, mrb_func_t cfunc)
{
  DEBUG_FPRINTLN("define method");
  mrb_proc *rproc = mrbc_rproc_alloc(vm, name);
  rproc->c_func = 1;  // c-func
  rproc->next = cls->procs;
  cls->procs = rproc;
  rproc->func = cfunc;
}


void mrbc_init_class_object(int z){
  DEBUG_FPRINTLN("define Object class");
  mrb_mvm* vm = get_vm();
  // Class
  mrbc_class_object = mrbc_define_class(vm,"Object", 0);
  // Methods
  mrbc_define_method(vm, mrbc_class_object, "puts", c_puts);
  //TODO
#if 0
  mrbc_define_method(vm, mrbc_class_object, "!", c_object_not);
  mrbc_define_method(vm, mrbc_class_object, "!=", c_object_neq);
  mrbc_define_method(vm, mrbc_class_object, "<=>", c_object_compare);
  mrbc_define_method(vm, mrbc_class_object, "to_s", c_object_to_s);
  mrbc_define_method(vm, mrbc_class_object, "new", c_object_new);
  mrbc_define_method(vm, mrbc_class_object, "class", c_object_class);
  mrbc_define_method(vm, mrbc_class_object, "attr_reader", c_object_attr_reader);
  mrbc_define_method(vm, mrbc_class_object, "attr_accessor", c_object_attr_accessor);
#endif
  
}

void mrbc_init_class(void)
{
  mrbc_init_class_object(0);
  mrbc_init_class_string(0);
  //TODO
#if 0
  mrbc_init_class_nil(0);
  mrbc_init_class_proc(0);
  mrbc_init_class_false(0);
  mrbc_init_class_true(0);

  mrbc_init_class_fixnum(0);
  mrbc_init_class_symbol(0);
  mrbc_init_class_array(0);
  mrbc_init_class_range(0);
  mrbc_init_class_hash(0);
#endif
}

