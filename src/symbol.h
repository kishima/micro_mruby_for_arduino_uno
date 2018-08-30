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
mrb_sym str_to_symid(const char *str);
const char *symid_to_str(mrb_sym sym_id);
void mrbc_init_class_symbol(struct VM *vm);

#endif

