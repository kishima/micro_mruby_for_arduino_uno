#ifndef MRBC_SRC_RGB_LCD_H_
#define MRBC_SRC_RGB_LCD_H_

void define_rgb_lcd_class();

//C functions for static proc table
void class_rbg_lcd_initialize(mrb_mvm *vm, mrb_value *v, int argc );
void class_rbg_lcd_clear(mrb_mvm *vm, mrb_value *v, int argc );
void class_rbg_lcd_set_cursor(mrb_mvm *vm, mrb_value *v, int argc );
void class_rbg_lcd_write(mrb_mvm *vm, mrb_value *v, int argc );
void class_rbg_lcd_set_rgb(mrb_mvm *vm, mrb_value *v, int argc );

#endif


