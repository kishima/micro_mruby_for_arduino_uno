/*! @file
  @brief
  Manage global objects.

  <pre>
  Copyright (C) 2015 Kyushu Institute of Technology.
  Copyright (C) 2015 Shimane IT Open-innovation Center.

  This file is distributed under BSD 3-Clause License.

  </pre>
*/

#ifndef MRBC_SRC_GLOBAL_H_
#define MRBC_SRC_GLOBAL_H_

#include "value.h"

void global_object_add(mrb_sym sym_id, mrb_value v);
mrb_value global_object_get(mrb_sym sym_id);

void const_object_add(mrb_sym sym_id, mrb_object *obj);
mrb_object const_object_get(mrb_sym sym_id);

#endif



