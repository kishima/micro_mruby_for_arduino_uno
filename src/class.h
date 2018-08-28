/*! @file
  @brief

  <pre>
  Copyright (C) 2015-2017 Kyushu Institute of Technology.
  Copyright (C) 2015-2017 Shimane IT Open-Innovation Center.

  This file is distributed under BSD 3-Clause License.


  </pre>
*/

#ifndef MRBC_SRC_CLASS_H_
#define MRBC_SRC_CLASS_H_

#include <stdint.h>
#include "micro_vm.h"
#include "value.h"

#ifdef __cplusplus
//extern "C" {
#endif

extern mrb_class *mrbc_class_object;

extern mrb_class *mrbc_class_proc;
extern mrb_class *mrbc_class_false;
extern mrb_class *mrbc_class_true;
extern mrb_class *mrbc_class_nil;
extern mrb_class *mrbc_class_array;
extern mrb_class *mrbc_class_fixnum;
extern mrb_class *mrbc_class_string;
extern mrb_class *mrbc_class_symbol;
extern mrb_class *mrbc_class_range;
extern mrb_class *mrbc_class_hash;

struct VM;

mrb_proc *find_method(struct VM *vm, mrb_value recv, mrb_sym sym_id);

void mrbc_init_class(void);
mrb_class * mrbc_define_class(mrb_mvm* vm, const char *name, mrb_class *super);
void mrbc_define_method(struct VM *vm, mrb_class *cls, const char *name, mrb_func_t func);

void mrbc_funcall(mrb_mvm *vm, const char *name, mrb_value *v, int argc);
void c_ineffect(mrb_mvm *vm, mrb_value v[], int argc);

#ifdef __cplusplus
//}
#endif
#endif
