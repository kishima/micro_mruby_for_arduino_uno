#ifndef MRBC_SRC_C_FIXNUM_H_
#define MRBC_SRC_C_FIXNUM_H_

#include <stdint.h>
#include "value.h"

void mrbc_init_class_fixnum();

void c_fixnum_bitref(mrb_mvm *vm, mrb_value v[], int argc);
void c_fixnum_negative(mrb_mvm *vm, mrb_value v[], int argc);
void c_fixnum_power(mrb_mvm *vm, mrb_value v[], int argc);
void c_fixnum_mod(mrb_mvm *vm, mrb_value v[], int argc);
void c_fixnum_and(mrb_mvm *vm, mrb_value v[], int argc);
void c_fixnum_or(mrb_mvm *vm, mrb_value v[], int argc);
void c_fixnum_xor(mrb_mvm *vm, mrb_value v[], int argc);
void c_fixnum_not(mrb_mvm *vm, mrb_value v[], int argc);
void c_fixnum_lshift(mrb_mvm *vm, mrb_value v[], int argc);
void c_fixnum_rshift(mrb_mvm *vm, mrb_value v[], int argc);
void c_fixnum_abs(mrb_mvm *vm, mrb_value v[], int argc);
void c_fixnum_times(mrb_mvm *vm, mrb_value v[], int argc);
void c_fixnum_chr(mrb_mvm *vm, mrb_value v[], int argc);
void c_fixnum_to_s(mrb_mvm *vm, mrb_value v[], int argc);


#endif