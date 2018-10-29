/* Irep table */
const unsigned char mmruby_code_irep_000[] PROGMEM= {
0x00,0x29,0x12,0x0B,0x01,0x80,0x00,0x11,0x01,0x80,0x40,0x20,0x00,0xBF,0xFF,0x83,
0x01,0x3F,0xFF,0x83,0x01,0x80,0x00,0x11,0x01,0x80,0x80,0x20,0x01,0x80,0x00,0x11,
0x02,0x00,0x00,0x3D,0x01,0x80,0xC0,0xA0,0x00,0x40,0x0D,0x97,0x01,0x80,0x40,0x01,
0x01,0x81,0x00,0xAD,0x00,0x80,0xC0,0x01,0x02,0x40,0x31,0x83,0x01,0x81,0x40,0xB6,
0x01,0xC0,0x00,0x99,0x00,0xBF,0xFF,0x83,0x01,0x80,0x00,0x11,0x02,0x00,0x40,0x01,
0x02,0xC0,0x04,0x83,0x02,0x01,0xC0,0xB0,0x02,0xC0,0x7C,0x83,0x02,0x02,0x00,0xA0,
0x02,0x80,0x40,0x01,0x03,0x40,0x04,0x83,0x02,0x81,0xC0,0xB0,0x03,0x40,0x7C,0x83,
0x02,0x82,0x00,0xA0,0x03,0x00,0x40,0x01,0x03,0xC0,0x04,0x83,0x03,0x01,0xC0,0xB0,
0x03,0xC0,0x7C,0x83,0x03,0x02,0x00,0xA0,0x01,0x81,0x81,0xA0,0x01,0x80,0x04,0x91,
0x02,0x40,0x31,0x83,0x01,0x82,0x80,0xA0,0x01,0x80,0x00,0x07,0x01,0xBF,0xF1,0x98,
0x01,0x80,0x00,0x05,0x00,0x00,0x00,0x4A,0x00,0x00,0x0F,0x48,0x65,0x6C,0x6C,0x6F,
0x20,0x6D,0x72,0x75,0x62,0x79,0x2F,0x63,0x21,0x21,0x0D,0x1E,0x1F,0x20,0x16,0x21,
0x22,0x23,0x24,0x0C,0x25,
};
const unsigned char* const mmruby_code_irep_table[] PROGMEM = {
  mmruby_code_irep_000,
};

/* Proc table */
const unsigned char mmruby_code_proc_table_size PROGMEM = 42;


const uint8_t mmruby_code_proc_table_Object[] PROGMEM = {
  38,1,
  39,2,
  40,3,
  41,4,
  16,5,
  42,6,
  43,7,
  44,8,
  0
};

const uint8_t mmruby_code_proc_table_Proc[] PROGMEM = {
  45,24,
  16,25,
  0
};

const uint8_t mmruby_code_proc_table_False[] PROGMEM = {
  16,27,
  0
};

const uint8_t mmruby_code_proc_table_True[] PROGMEM = {
  16,28,
  0
};

const uint8_t mmruby_code_proc_table_Nil[] PROGMEM = {
  16,26,
  0
};

const uint8_t mmruby_code_proc_table_Array[] PROGMEM = {
  0
};

const uint8_t mmruby_code_proc_table_Fixnum[] PROGMEM = {
  25,9,
  46,10,
  47,11,
  36,12,
  48,13,
  49,14,
  50,15,
  51,16,
  24,17,
  29,18,
  52,19,
  53,20,
  54,21,
  55,22,
  16,23,
  0
};

const uint8_t mmruby_code_proc_table_String[] PROGMEM = {
  56,30,
  16,31,
  57,32,
  58,33,
  0
};

const uint8_t mmruby_code_proc_table_Symbol[] PROGMEM = {
  16,29,
  0
};

const uint8_t mmruby_code_proc_table_Range[] PROGMEM = {
  0
};

const uint8_t mmruby_code_proc_table_Arduino[] PROGMEM = {
  37,34,
  59,35,
  60,36,
  61,37,
  0
};

const uint8_t mmruby_code_proc_table_RGB_LCD[] PROGMEM = {
  30,38,
  31,39,
  62,40,
  32,41,
  34,42,
  0
};

inline mrb_func_t find_c_funcs_by_no(short no){
  mrb_func_t func=0;
  switch(no){
    case 1: func = c_puts; break;
    case 2: func = c_object_not; break;
    case 3: func = c_object_neq; break;
    case 4: func = c_object_compare; break;
    case 5: func = c_object_to_s; break;
    case 6: func = c_object_class; break;
    case 7: func = c_object_attr_reader; break;
    case 8: func = c_object_attr_accessor; break;
    case 9: func = c_fixnum_bitref; break;
    case 10: func = c_fixnum_negative; break;
    case 11: func = c_fixnum_power; break;
    case 12: func = c_fixnum_mod; break;
    case 13: func = c_fixnum_and; break;
    case 14: func = c_fixnum_or; break;
    case 15: func = c_fixnum_xor; break;
    case 16: func = c_fixnum_not; break;
    case 17: func = c_fixnum_lshift; break;
    case 18: func = c_fixnum_rshift; break;
    case 19: func = c_fixnum_abs; break;
    case 20: func = c_fixnum_times; break;
    case 21: func = c_fixnum_chr; break;
    case 22: func = c_ineffect; break;
    case 23: func = c_fixnum_to_s; break;
    case 24: func = c_proc_call; break;
    case 25: func = c_proc_to_s; break;
    case 26: func = c_nil_to_s; break;
    case 27: func = c_false_to_s; break;
    case 28: func = c_true_to_s; break;
    case 29: func = c_true_to_s; break;
    case 30: func = c_all_symbols; break;
    case 31: func = c_to_s; break;
    case 32: func = c_to_s; break;
    case 33: func = c_ineffect; break;
    case 34: func = class_arduino_delay; break;
    case 35: func = class_arduino_pin_mode; break;
    case 36: func = class_arduino_digital_write; break;
    case 37: func = class_arduino_digital_read; break;
    case 38: func = class_rbg_lcd_initialize; break;
    case 39: func = class_rbg_lcd_clear; break;
    case 40: func = class_rbg_lcd_set_cursor; break;
    case 41: func = class_rbg_lcd_write; break;
    case 42: func = class_rbg_lcd_set_rgb; break;
    default: break;
  }
  return func;
}


/* Symbol table */
#include "symbol_ids.h"

const unsigned char mmruby_code_symbol_table_size PROGMEM = 63;
const char mmruby_code_symbol_ref00[] PROGMEM ="";
const char mmruby_code_symbol_ref01[] PROGMEM ="Object";
const char mmruby_code_symbol_ref02[] PROGMEM ="String";
const char mmruby_code_symbol_ref03[] PROGMEM ="Nil";
const char mmruby_code_symbol_ref04[] PROGMEM ="Proc";
const char mmruby_code_symbol_ref05[] PROGMEM ="False";
const char mmruby_code_symbol_ref06[] PROGMEM ="True";
const char mmruby_code_symbol_ref07[] PROGMEM ="Fixnum";
const char mmruby_code_symbol_ref08[] PROGMEM ="Symbol";
const char mmruby_code_symbol_ref09[] PROGMEM ="Array";
const char mmruby_code_symbol_ref0a[] PROGMEM ="Range";
const char mmruby_code_symbol_ref0b[] PROGMEM ="Hash";
const char mmruby_code_symbol_ref0c[] PROGMEM ="Arduino";
const char mmruby_code_symbol_ref0d[] PROGMEM ="RGB_LCD";
const char mmruby_code_symbol_ref0e[] PROGMEM ="true";
const char mmruby_code_symbol_ref0f[] PROGMEM ="false";
const char mmruby_code_symbol_ref10[] PROGMEM ="to_s";
const char mmruby_code_symbol_ref11[] PROGMEM ="INPUT";
const char mmruby_code_symbol_ref12[] PROGMEM ="OUTPUT";
const char mmruby_code_symbol_ref13[] PROGMEM ="INPUT_PULLUP";
const char mmruby_code_symbol_ref14[] PROGMEM ="HIGH";
const char mmruby_code_symbol_ref15[] PROGMEM ="LOW";
const char mmruby_code_symbol_ref16[] PROGMEM ="+";
const char mmruby_code_symbol_ref17[] PROGMEM ="===";
const char mmruby_code_symbol_ref18[] PROGMEM ="<<";
const char mmruby_code_symbol_ref19[] PROGMEM ="[]";
const char mmruby_code_symbol_ref1a[] PROGMEM ="[]=";
const char mmruby_code_symbol_ref1b[] PROGMEM ="()";
const char mmruby_code_symbol_ref1c[] PROGMEM ="-";
const char mmruby_code_symbol_ref1d[] PROGMEM =">>";
const char mmruby_code_symbol_ref1e[] PROGMEM ="initialize";
const char mmruby_code_symbol_ref1f[] PROGMEM ="clear";
const char mmruby_code_symbol_ref20[] PROGMEM ="write";
const char mmruby_code_symbol_ref21[] PROGMEM =">=";
const char mmruby_code_symbol_ref22[] PROGMEM ="set_rgb";
const char mmruby_code_symbol_ref23[] PROGMEM ="*";
const char mmruby_code_symbol_ref24[] PROGMEM ="%";
const char mmruby_code_symbol_ref25[] PROGMEM ="delay";
const char mmruby_code_symbol_ref26[] PROGMEM ="puts";
const char mmruby_code_symbol_ref27[] PROGMEM ="!";
const char mmruby_code_symbol_ref28[] PROGMEM ="!=";
const char mmruby_code_symbol_ref29[] PROGMEM ="<=>";
const char mmruby_code_symbol_ref2a[] PROGMEM ="class";
const char mmruby_code_symbol_ref2b[] PROGMEM ="attr_reader";
const char mmruby_code_symbol_ref2c[] PROGMEM ="attr_accessor";
const char mmruby_code_symbol_ref2d[] PROGMEM ="call";
const char mmruby_code_symbol_ref2e[] PROGMEM ="-@";
const char mmruby_code_symbol_ref2f[] PROGMEM ="**";
const char mmruby_code_symbol_ref30[] PROGMEM ="&";
const char mmruby_code_symbol_ref31[] PROGMEM ="|";
const char mmruby_code_symbol_ref32[] PROGMEM ="^";
const char mmruby_code_symbol_ref33[] PROGMEM ="~";
const char mmruby_code_symbol_ref34[] PROGMEM ="abs";
const char mmruby_code_symbol_ref35[] PROGMEM ="times";
const char mmruby_code_symbol_ref36[] PROGMEM ="chr";
const char mmruby_code_symbol_ref37[] PROGMEM ="to_i";
const char mmruby_code_symbol_ref38[] PROGMEM ="all_symbols";
const char mmruby_code_symbol_ref39[] PROGMEM ="id2name";
const char mmruby_code_symbol_ref3a[] PROGMEM ="to_sym";
const char mmruby_code_symbol_ref3b[] PROGMEM ="pin_mode";
const char mmruby_code_symbol_ref3c[] PROGMEM ="digital_write";
const char mmruby_code_symbol_ref3d[] PROGMEM ="digital_read";
const char mmruby_code_symbol_ref3e[] PROGMEM ="set_cursor";

const char* const mmruby_code_symbol_table[] PROGMEM = {
  mmruby_code_symbol_ref00,
  mmruby_code_symbol_ref01,
  mmruby_code_symbol_ref02,
  mmruby_code_symbol_ref03,
  mmruby_code_symbol_ref04,
  mmruby_code_symbol_ref05,
  mmruby_code_symbol_ref06,
  mmruby_code_symbol_ref07,
  mmruby_code_symbol_ref08,
  mmruby_code_symbol_ref09,
  mmruby_code_symbol_ref0a,
  mmruby_code_symbol_ref0b,
  mmruby_code_symbol_ref0c,
  mmruby_code_symbol_ref0d,
  mmruby_code_symbol_ref0e,
  mmruby_code_symbol_ref0f,
  mmruby_code_symbol_ref10,
  mmruby_code_symbol_ref11,
  mmruby_code_symbol_ref12,
  mmruby_code_symbol_ref13,
  mmruby_code_symbol_ref14,
  mmruby_code_symbol_ref15,
  mmruby_code_symbol_ref16,
  mmruby_code_symbol_ref17,
  mmruby_code_symbol_ref18,
  mmruby_code_symbol_ref19,
  mmruby_code_symbol_ref1a,
  mmruby_code_symbol_ref1b,
  mmruby_code_symbol_ref1c,
  mmruby_code_symbol_ref1d,
  mmruby_code_symbol_ref1e,
  mmruby_code_symbol_ref1f,
  mmruby_code_symbol_ref20,
  mmruby_code_symbol_ref21,
  mmruby_code_symbol_ref22,
  mmruby_code_symbol_ref23,
  mmruby_code_symbol_ref24,
  mmruby_code_symbol_ref25,
  mmruby_code_symbol_ref26,
  mmruby_code_symbol_ref27,
  mmruby_code_symbol_ref28,
  mmruby_code_symbol_ref29,
  mmruby_code_symbol_ref2a,
  mmruby_code_symbol_ref2b,
  mmruby_code_symbol_ref2c,
  mmruby_code_symbol_ref2d,
  mmruby_code_symbol_ref2e,
  mmruby_code_symbol_ref2f,
  mmruby_code_symbol_ref30,
  mmruby_code_symbol_ref31,
  mmruby_code_symbol_ref32,
  mmruby_code_symbol_ref33,
  mmruby_code_symbol_ref34,
  mmruby_code_symbol_ref35,
  mmruby_code_symbol_ref36,
  mmruby_code_symbol_ref37,
  mmruby_code_symbol_ref38,
  mmruby_code_symbol_ref39,
  mmruby_code_symbol_ref3a,
  mmruby_code_symbol_ref3b,
  mmruby_code_symbol_ref3c,
  mmruby_code_symbol_ref3d,
  mmruby_code_symbol_ref3e,
};

