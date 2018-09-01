#ifndef MRBC_SRC_C_OBJECT_H_
#define MRBC_SRC_C_OBJECT_H_

struct VM;

void mrbc_init_class_object();

//C functions for static proc table
void c_puts(mrb_mvm *vm, mrb_value v[], int argc);
void c_object_not(mrb_mvm *vm, mrb_value v[], int argc);
void c_object_neq(mrb_mvm *vm, mrb_value v[], int argc);
void c_object_compare(mrb_mvm *vm, mrb_value v[], int argc);
void c_object_class(mrb_mvm *vm, mrb_value v[], int argc);
void c_object_new(mrb_mvm *vm, mrb_value v[], int argc);
void c_object_getiv(mrb_mvm *vm, mrb_value v[], int argc);
void c_object_setiv(mrb_mvm *vm, mrb_value v[], int argc);
void c_object_attr_reader(mrb_mvm *vm, mrb_value v[], int argc);
void c_object_attr_accessor(mrb_mvm *vm, mrb_value v[], int argc);
void c_object_to_s(mrb_mvm *vm, mrb_value v[], int argc);




#endif

