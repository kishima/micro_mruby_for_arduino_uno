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

#ifdef __cplusplus
extern "C" {
#endif

inline void *mrbc_alloc(const mrb_mvm *vm, unsigned int size);
inline void *mrbc_realloc(const mrb_mvm *vm, void *ptr, unsigned int size);
inline void mrbc_free(const mrb_mvm *vm, void *ptr);


#ifdef __cplusplus
}
#endif
#endif
