/*! @file
  @brief
  Global configration of mruby/c VM's

  <pre>
  Copyright (C) 2015 Kyushu Institute of Technology.
  Copyright (C) 2015 Shimane IT Open-Innovation Center.

  This file is distributed under BSD 3-Clause License.


  </pre>
*/

#ifndef MRBC_SRC_VM_CONFIG_H_
#define MRBC_SRC_VM_CONFIG_H_


#define CODE_PREFIX "mmruby_code_"
#define CODE_LEN 4
#define MAX_LITERAL_LEN 24
#define MAX_SYMBOL_LEN 16
#define INIT_SYMBOL_TABLE_LEN 10

#define MAX_GLOBAL_OBJECT_SIZE 20
#define MAX_REGS_SIZE 16
#define MAX_CALLINFO_SIZE 10

#endif

