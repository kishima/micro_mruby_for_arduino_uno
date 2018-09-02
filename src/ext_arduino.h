#ifndef MRBC_SRC_ARDUINO_H_
#define MRBC_SRC_ARDUINO_H_

void mrbc_init_class_arduino();

//C functions for static proc table
void class_arduino_delay(mrb_mvm *vm, mrb_value *v, int argc );
void class_arduino_pin_mode(mrb_mvm *vm, mrb_value *v, int argc );
void class_arduino_digital_write(mrb_mvm *vm, mrb_value *v, int argc );
void class_arduino_digital_read(mrb_mvm *vm, mrb_value *v, int argc );

#endif
