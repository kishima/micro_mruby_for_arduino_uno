/*! @file
  @brief
  mrubyc memory management.

  <pre>
  Copyright (C) 2015-2018 Kyushu Institute of Technology.
  Copyright (C) 2015-2018 Shimane IT Open-Innovation Center.

  This file is distributed under BSD 3-Clause License.

  Memory management for objects in mruby/c.

  </pre>
*/

#ifndef MRBC_SRC_ALLOC_H_
#define MRBC_SRC_ALLOC_H_

#include "micro_vm.h"

void* mrbc_alloc(const mrb_mvm *vm, unsigned int size);
void* mrbc_realloc(const mrb_mvm *vm, void *ptr, unsigned int size);
void  mrbc_free(const mrb_mvm *vm, void *ptr);

void* mrbc_raw_alloc(unsigned int size);
void* mrbc_raw_realloc(void* ptr,unsigned int size);
void  mrbc_raw_free(void* ptr);

#endif
