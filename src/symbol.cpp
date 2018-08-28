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
#include "symbol.h"


mrb_value mrbc_symbol_new(struct VM *vm, const char *str)
{
  mrb_value ret = {.tt = MRB_TT_SYMBOL};
  //TODO
#if 0
  uint16_t h = calc_hash(str);
  mrb_sym sym_id = search_index(h, str);

  if( sym_id >= 0 ) {
    ret.i = sym_id;
    return ret;		// already exist.
  }

  // create symbol object dynamically.
  int size = strlen(str) + 1;
  char *buf = mrbc_raw_alloc(size);
  if( buf == NULL ) return ret;		// ENOMEM raise?

  memcpy(buf, str, size);
  ret.i = add_index( h, buf );
#endif
  return ret;
}


mrb_sym str_to_symid(const char *str)
{
  //TODO
#if 0
  uint16_t h = calc_hash(str);
  mrb_sym sym_id = search_index(h, str);
  if( sym_id >= 0 ) return sym_id;

  return add_index( h, str );
#endif
  return 0;
}

