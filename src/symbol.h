/*! @file
  @brief
  mruby/c Symbol class

  <pre>
  Copyright (C) 2015-2018 Kyushu Institute of Technology.
  Copyright (C) 2015-2018 Shimane IT Open-Innovation Center.

  This file is distributed under BSD 3-Clause License.

  </pre>
*/

#ifndef MRBC_SRC_SYMBOL_H_
#define MRBC_SRC_SYMBOL_H_

#include "value.h"

#define MAX_SYMBOL (256)
#define INVALID_SYMBOL (MAX_SYMBOL-1)

struct VM;

void init_symbol_table(void);
mrb_value mrbc_symbol_new(struct VM *vm, const char *str);
mrb_value mrbc_string_new_cstr(const char *src);
mrb_sym str_to_symid(const char *str);
const char *symid_to_str(mrb_sym sym_id);
void mrbc_init_class_symbol(void);

void c_all_symbols(mrb_mvm *vm, mrb_value v[], int argc);
void c_to_s(mrb_mvm *vm, mrb_value v[], int argc);
void c_equal3(mrb_mvm *vm, mrb_value v[], int argc);

static inline const char * mrbc_symbol_cstr(const mrb_value *v)
{
  return symid_to_str(v->i);
}

#endif

