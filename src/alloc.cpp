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

#include <stdlib.h>
#include "console.h"
#include "alloc.h"

#ifdef MMRUBY_DEBUG_ENABLE
static int16_t alloc_mem_size=0;
#endif

void * mrbc_alloc(const mrb_mvm *vm, unsigned int size)
{
  void *p = malloc(size);
#ifdef MMRUBY_DEBUG_ENABLE
 alloc_mem_size += size;
 cprintf(">>ALLOC %p:%d\n",p,size);
 cprintf("alloc total=%d\n",alloc_mem_size);
#endif
  return p;
}

void * mrbc_realloc(const mrb_mvm *vm, void *ptr, unsigned int size)
{
#ifdef MMRUBY_DEBUG_ENABLE
 alloc_mem_size += size;
 cprintf(">>REALC %p:%d\n",ptr,size);
 cprintf("alloc total=%d\n",alloc_mem_size);
#endif
  return realloc(ptr, size);
}

void mrbc_free(const mrb_mvm *vm, void *ptr)
{
#ifdef MMRUBY_DEBUG_ENABLE
  cprintf(">>FREE  %p\n",ptr);
#endif
  free(ptr);
}

