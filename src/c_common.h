#ifndef MRBC_SRC_C_COMMON_H_
#define MRBC_SRC_C_COMMON_H_

struct VM;

void mrbc_init_class_proc(void);
void mrbc_init_class_nil(void);
void mrbc_init_class_false(void);
void mrbc_init_class_true(void);

void c_proc_call(mrb_mvm *vm, mrb_value v[], int argc);
void c_proc_to_s(mrb_mvm *vm, mrb_value v[], int argc);
void c_nil_to_s(mrb_mvm *vm, mrb_value v[], int argc);
void c_false_to_s(mrb_mvm *vm, mrb_value v[], int argc);
void c_true_to_s(mrb_mvm *vm, mrb_value v[], int argc);

#endif

