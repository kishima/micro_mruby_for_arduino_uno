#ifndef MRBC_SRC_C_OBJECT_H_
#define MRBC_SRC_C_OBJECT_H_

struct VM;

void mrbc_init_class_object();

//C functions for static proc table
void c_puts(mrb_mvm *vm, mrb_value v[], int argc);

#endif

