/*
  ext_rgb_lcd.cpp
  Defining extension methods of Arduino
  Copyright (c) 2018, katsuhiko kageyama All rights reserved.
*/
#include "ext.h"

#ifdef USE_RGB_LCD

#include <Wire.h>
#include <rgb_lcd.h>

#include "value.h"
#include "micro_vm.h"
#include "class.h"
#include "alloc.h"
#include "c_string.h"
#include "global.h"
#include "symbol.h"
#include "console.h"
#include "debug.h"
#include "avr_access.h"
#include "symbol_ids.h"

static rgb_lcd* lcd=NULL;

void class_rbg_lcd_initialize(mrb_mvm *vm, mrb_value *v, int argc ){
  if(lcd!=NULL){
    return;
  }
  lcd = new rgb_lcd();
  lcd->begin(16, 2);
  lcd->display();
}

void class_rbg_lcd_clear(mrb_mvm *vm, mrb_value *v, int argc ){
  if(lcd==NULL){
    SET_NIL_RETURN();
    return;
  }
  lcd->clear();
  //keep self
}

void class_rbg_lcd_set_cursor(mrb_mvm *vm, mrb_value *v, int argc ){
  if(lcd==NULL){
    SET_NIL_RETURN();
    return;
  }
  //read arg
  if(argc!=2){
    SET_NIL_RETURN();
    return;
  }
  int8_t y = GET_INT_ARG(1);
  int8_t x = GET_INT_ARG(2);
  cprintf("x,y %d,%d\n", x,y);

  lcd->setCursor(y,x);
  //keep self
}

void class_rbg_lcd_write(mrb_mvm *vm, mrb_value *v, int argc ){
  if(lcd==NULL){
    SET_NIL_RETURN();
    return;
  }
  const char* text = (const char *)GET_STRING_ARG(1);
  lcd->print(text);
  //keep self
}

void class_rbg_lcd_set_rgb(mrb_mvm *vm, mrb_value *v, int argc ){
  if(lcd==NULL){
    SET_NIL_RETURN();
    return;
  }
  int16_t r = GET_INT_ARG(1);
  int16_t g = GET_INT_ARG(2);
  int16_t b = GET_INT_ARG(3);

  lcd->setRGB(r,g,b);

  //keep self
}

void define_rgb_lcd_class(void){
  mrb_class *class_rbg_lcd;
  class_rbg_lcd = mrbc_define_class((char*)MRBC_SSYM_RGB_LCD, mrbc_class_object);
  /*
  mrbc_define_method(0, class_rbg_lcd, "initialize", class_rbg_lcd_initialize);
  mrbc_define_method(0, class_rbg_lcd, "clear", class_rbg_lcd_clear);
  mrbc_define_method(0, class_rbg_lcd, "set_cursor", class_rbg_lcd_set_cursor);
  mrbc_define_method(0, class_rbg_lcd, "write", class_rbg_lcd_write);
  mrbc_define_method(0, class_rbg_lcd, "set_rgb", class_rbg_lcd_set_rgb);
  */
}

#endif //USE_RGB_LCD

