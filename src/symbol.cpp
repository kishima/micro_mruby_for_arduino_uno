/*! @file
  @brief
  mruby/c Symbol class

  <pre>
  Copyright (C) 2015-2018 Kyushu Institute of Technology.
  Copyright (C) 2015-2018 Shimane IT Open-Innovation Center.

  This file is distributed under BSD 3-Clause License.

  </pre>
*/

#include "vm_config.h"
#include <stdint.h>
#include <string.h>
#include "avr_access.h"
#include "class.h"
#include "symbol.h"
#include "alloc.h"
#include "console.h"
#include "debug.h"

static char** symbol_table = NULL;
static uint8_t sym_tbl_cnt  = 0;
static uint8_t sym_tbl_size = 0;
static uint8_t static_sym_tbl_size = 0;

static char global_sym_name_buff[MAX_SYMBOL_LEN];

void init_symbol_table(void){
  //static table
  static_sym_tbl_size = get_max_static_symbol_id();
  //dynamic table
  sym_tbl_size = INIT_SYMBOL_TABLE_LEN;
  symbol_table = (char**)mrbc_raw_alloc( sizeof(char*) * sym_tbl_size );
}

mrb_sym add_index(const char* str ){
  if(sym_tbl_cnt+static_sym_tbl_size>=MAX_SYMBOL-1){ //Total table size must be less than MAX_SYMBOL
    return INVALID_SYMBOL;
  }

  if(sym_tbl_cnt>=sym_tbl_size){
    //extend table
    sym_tbl_size++;
    DEBUG_FPRINTLN("Extend SymTbl!");
    symbol_table = (char**)mrbc_raw_realloc(symbol_table, sizeof(char*)*sym_tbl_size);
  }
  symbol_table[sym_tbl_cnt] = (char*)mrbc_raw_alloc(strlen(str)+1);
  strcpy(symbol_table[sym_tbl_cnt],str);
  sym_tbl_cnt++;
  return sym_tbl_cnt-1;
}

mrb_sym search_index_dynamic(const char* str){
  int i;
  for(i=0;i<sym_tbl_cnt;i++){
    if(0 == strcmp(str, symbol_table[i])){
      return i;
    }
  }
  return INVALID_SYMBOL;
}

inline mrb_sym search_index(const char* str){
  mrb_sym sym_id = search_index_static(str);
  if(INVALID_SYMBOL!=sym_id) return sym_id;
  
  sym_id = search_index_dynamic(str);
  if(INVALID_SYMBOL!=sym_id) return sym_id + static_sym_tbl_size;

  return INVALID_SYMBOL;
}

mrb_value mrbc_symbol_new(struct VM *vm, const char *str)
{
  mrb_value ret = {.tt = MRB_TT_SYMBOL};
  mrb_sym sym_id = search_index(str);
  if(INVALID_SYMBOL!=sym_id){
    ret.i = sym_id;
    return ret;
  }

  // create symbol object dynamically.
  int size = strlen(str) + 1;
  char *buf = (char*)mrbc_raw_alloc(size);
  if( buf == NULL ) return ret; 	// ENOMEM raise?

  memcpy(buf, str, size);
  ret.i = add_index( buf );
  return ret;
}

mrb_sym str_to_symid(const char *str)
{
  mrb_sym sym_id = search_index(str);
  if(INVALID_SYMBOL!=sym_id) return sym_id;

  return add_index( str );
}


const char * symid_to_str(mrb_sym sym_id)
{
  const char * str;
  if( sym_id < 0 ) return NULL;
  if( sym_id < static_sym_tbl_size){
     copy_symbol_str(global_sym_name_buff,sym_id);
     return global_sym_name_buff;
  }
  return symbol_table[sym_id-static_sym_tbl_size];
}

/* Methods in Symbol class */
void c_all_symbols(mrb_mvm *vm, mrb_value v[], int argc)
{
  //TODO
#if 0
  mrb_value ret = mrbc_array_new(vm, sym_index_pos);

  int i;
  for( i = 0; i < sym_index_pos; i++ ) {
    mrb_value sym1 = {.tt = MRB_TT_SYMBOL};
    sym1.i = i;
    mrbc_array_push(&ret, &sym1);
  }
  SET_RETURN(ret);
#endif
}

void c_to_s(mrb_mvm *vm, mrb_value v[], int argc)
{
  v[0] = mrbc_string_new_cstr( symid_to_str(v[0].i));
}

void c_equal3(mrb_mvm *vm, mrb_value v[], int argc)
{
  if( mrbc_compare(&v[0], &v[1]) == 0 ) {
    SET_TRUE_RETURN();
  } else {
    SET_FALSE_RETURN();
  }
}

void mrbc_init_class_symbol(void){
  mrbc_class_symbol = mrbc_define_class("Symbol", 0);
  //methods
  //all_symbols
  //to_s
  //id2name
  //to_sym
  //===
}

