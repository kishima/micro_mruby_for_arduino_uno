#include "vm_config.h"
#include "value.h"
#include "debug.h"
#include "global.h"

typedef enum {
  MRBC_GLOBAL_OBJECT = 1,
  MRBC_CONST_OBJECT,
} mrbc_globaltype;

typedef struct GLOBAL_OBJECT {
  mrbc_globaltype gtype : 8;
  mrb_sym sym_id;
  mrb_object obj;
} mrb_globalobject;

// max of global object in mrbc_global[]
static int global_end;
static mrb_globalobject mrbc_global[MAX_GLOBAL_OBJECT_SIZE];

//
void  mrbc_init_global(void)
{
  global_end = 0;
}

/* search */
/* linear search is not efficient! */
/* TODO: Use binary search */
static int search_global_object(mrb_sym sym_id, mrbc_globaltype gtype)
{
  int i;
  for( i=0 ; i<global_end ; i++ ){
    mrb_globalobject *obj = &mrbc_global[i];
    if( obj->sym_id == sym_id && obj->gtype == gtype ) return i;
  }
  return -1;
}

/* add */
/* TODO: Check reference count */
void global_object_add(mrb_sym sym_id, mrb_value v)
{
  int index = search_global_object(sym_id, MRBC_GLOBAL_OBJECT);
  if( index == -1 ) {
    index = global_end++;
    //assert( index < MAX_GLOBAL_OBJECT_SIZE );	// maybe raise ex
  } else {
    mrbc_release( &(mrbc_global[index].obj) );
  }

  mrbc_global[index].gtype = MRBC_GLOBAL_OBJECT;
  mrbc_global[index].sym_id = sym_id;
  mrbc_global[index].obj = v;
  mrbc_dup( &v );
}

/* add const */
/* TODO: Check reference count */
/* TODO: Integrate with global_add */
void const_object_add(mrb_sym sym_id, mrb_object *obj)
{
  int index = search_global_object(sym_id, MRBC_CONST_OBJECT);
  if( index == -1 ){
    index = global_end;
    global_end++;
    //assert( index < MAX_GLOBAL_OBJECT_SIZE );	// maybe raise ex
  } else {
    // warning: already initialized constant.
    mrbc_release( &(mrbc_global[index].obj) );
  }
  mrbc_global[index].gtype = MRBC_CONST_OBJECT;
  mrbc_global[index].sym_id = sym_id;
  mrbc_global[index].obj = *obj;
  mrbc_dup( obj );
}

/* get */
mrb_value global_object_get(mrb_sym sym_id)
{
  int index = search_global_object(sym_id, MRBC_GLOBAL_OBJECT);
  if( index >= 0 ){
    mrbc_dup( &mrbc_global[index].obj );
    return mrbc_global[index].obj;
  } else {
    DEBUG_FPRINTLN("OBJ NOTFOUND");
    return mrb_nil_value();
  }
}

/* get const */
/* TODO: Integrate with get_global_object */
mrb_object const_object_get(mrb_sym sym_id)
{
  int index = search_global_object(sym_id, MRBC_CONST_OBJECT);
  if( index >= 0 ){
    mrbc_dup( &mrbc_global[index].obj );
    return mrbc_global[index].obj;
  } else {
    return mrb_nil_value();
  }
}

