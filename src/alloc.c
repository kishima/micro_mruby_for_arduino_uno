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

#include <stddef.h>
#include "alloc.h"

inline void * mrbc_alloc(const mrb_mvm *vm, unsigned int size)
{
  return malloc(size);
}

inline void * mrbc_realloc(const mrb_mvm *vm, void *ptr, unsigned int size)
{
  return realloc(ptr, size);
}

inline void mrbc_free(const mrb_mvm *vm, void *ptr)
{
  free(ptr);
}


