/* Irep table */
const unsigned char mmruby_code_irep_000[] PROGMEM= {
0x00,0x1C,0x0B,0x09,0x01,0x80,0x00,0x06,0x02,0x00,0x00,0x3D,0x01,0x80,0x00,0xA0,
0x00,0xC0,0x06,0x03,0x01,0x80,0x00,0x91,0x02,0x00,0x40,0x01,0x02,0x80,0x01,0x84,
0x01,0x80,0x81,0x20,0x01,0x00,0x02,0x04,0x00,0x40,0x07,0x17,0x01,0x80,0x80,0x01,
0x02,0x00,0x02,0x04,0x01,0x81,0x40,0xB2,0x01,0xC0,0x01,0x19,0x01,0x00,0x03,0x04,
0x00,0x40,0x00,0x97,0x01,0x00,0x02,0x04,0x01,0x80,0x00,0x91,0x02,0x00,0x40,0x01,
0x02,0x80,0x80,0x01,0x01,0x81,0xC1,0x20,0x01,0x80,0x00,0x91,0x02,0x40,0xF9,0x83,
0x01,0x82,0x00,0xA0,0x01,0x80,0x00,0x07,0x01,0xBF,0xF8,0x18,0x01,0x80,0x00,0x05,
0x00,0x00,0x00,0x4A,0x00,0x00,0x08,0x4C,0x45,0x44,0x20,0x54,0x65,0x73,0x74,0x15,
0x0C,0x16,0x17,0x18,0x19,0x1A,0x1B,0x1C,
};
const unsigned char* const mmruby_code_irep_table[] PROGMEM = {
  mmruby_code_irep_000,
};

/* Proc table */
const unsigned char mmruby_code_proc_table_size PROGMEM = 37;


const uint8_t mmruby_code_proc_table_Object[] PROGMEM = {
  21,1,
  29,2,
  30,3,
  31,4,
  32,5,
  33,6,
  34,7,
  35,8,
  0
};

const uint8_t mmruby_code_proc_table_Proc[] PROGMEM = {
  36,24,
  32,25,
  0
};

const uint8_t mmruby_code_proc_table_False[] PROGMEM = {
  32,27,
  0
};

const uint8_t mmruby_code_proc_table_True[] PROGMEM = {
  32,28,
  0
};

const uint8_t mmruby_code_proc_table_Nil[] PROGMEM = {
  32,26,
  0
};

const uint8_t mmruby_code_proc_table_Array[] PROGMEM = {
  0
};

const uint8_t mmruby_code_proc_table_Fixnum[] PROGMEM = {
  16,9,
  37,10,
  38,11,
  39,12,
  40,13,
  41,14,
  42,15,
  43,16,
  15,17,
  20,18,
  44,19,
  45,20,
  46,21,
  47,22,
  32,23,
  0
};

const uint8_t mmruby_code_proc_table_String[] PROGMEM = {
  48,30,
  32,31,
  49,32,
  50,33,
  0
};

const uint8_t mmruby_code_proc_table_Symbol[] PROGMEM = {
  32,29,
  0
};

const uint8_t mmruby_code_proc_table_Range[] PROGMEM = {
  0
};

const uint8_t mmruby_code_proc_table_Arduino[] PROGMEM = {
  28,34,
  22,35,
  27,36,
  51,37,
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
    default: break;
  }
  return func;
}


/* Symbol table */
const unsigned char mmruby_code_symbol_table_size PROGMEM = 52;
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
const char mmruby_code_symbol_ref0d[] PROGMEM ="+";
const char mmruby_code_symbol_ref0e[] PROGMEM ="===";
const char mmruby_code_symbol_ref0f[] PROGMEM ="<<";
const char mmruby_code_symbol_ref10[] PROGMEM ="[]";
const char mmruby_code_symbol_ref11[] PROGMEM ="[]=";
const char mmruby_code_symbol_ref12[] PROGMEM ="()";
const char mmruby_code_symbol_ref13[] PROGMEM ="-";
const char mmruby_code_symbol_ref14[] PROGMEM =">>";
const char mmruby_code_symbol_ref15[] PROGMEM ="puts";
const char mmruby_code_symbol_ref16[] PROGMEM ="pin_mode";
const char mmruby_code_symbol_ref17[] PROGMEM ="OUTPUT";
const char mmruby_code_symbol_ref18[] PROGMEM ="LOW";
const char mmruby_code_symbol_ref19[] PROGMEM ="==";
const char mmruby_code_symbol_ref1a[] PROGMEM ="HIGH";
const char mmruby_code_symbol_ref1b[] PROGMEM ="digital_write";
const char mmruby_code_symbol_ref1c[] PROGMEM ="delay";
const char mmruby_code_symbol_ref1d[] PROGMEM ="!";
const char mmruby_code_symbol_ref1e[] PROGMEM ="!=";
const char mmruby_code_symbol_ref1f[] PROGMEM ="<=>";
const char mmruby_code_symbol_ref20[] PROGMEM ="to_s";
const char mmruby_code_symbol_ref21[] PROGMEM ="class";
const char mmruby_code_symbol_ref22[] PROGMEM ="attr_reader";
const char mmruby_code_symbol_ref23[] PROGMEM ="attr_accessor";
const char mmruby_code_symbol_ref24[] PROGMEM ="call";
const char mmruby_code_symbol_ref25[] PROGMEM ="-@";
const char mmruby_code_symbol_ref26[] PROGMEM ="**";
const char mmruby_code_symbol_ref27[] PROGMEM ="%";
const char mmruby_code_symbol_ref28[] PROGMEM ="&";
const char mmruby_code_symbol_ref29[] PROGMEM ="|";
const char mmruby_code_symbol_ref2a[] PROGMEM ="^";
const char mmruby_code_symbol_ref2b[] PROGMEM ="~";
const char mmruby_code_symbol_ref2c[] PROGMEM ="abs";
const char mmruby_code_symbol_ref2d[] PROGMEM ="times";
const char mmruby_code_symbol_ref2e[] PROGMEM ="chr";
const char mmruby_code_symbol_ref2f[] PROGMEM ="to_i";
const char mmruby_code_symbol_ref30[] PROGMEM ="all_symbols";
const char mmruby_code_symbol_ref31[] PROGMEM ="id2name";
const char mmruby_code_symbol_ref32[] PROGMEM ="to_sym";
const char mmruby_code_symbol_ref33[] PROGMEM ="digital_read";

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
};

/* Symbol IDs */
#define MRBC_SSYM_Object (1) // Object
#define MRBC_SSYM_String (2) // String
#define MRBC_SSYM_Nil (3) // Nil
#define MRBC_SSYM_Proc (4) // Proc
#define MRBC_SSYM_False (5) // False
#define MRBC_SSYM_True (6) // True
#define MRBC_SSYM_Fixnum (7) // Fixnum
#define MRBC_SSYM_Symbol (8) // Symbol
#define MRBC_SSYM_Array (9) // Array
#define MRBC_SSYM_Range (10) // Range
#define MRBC_SSYM_Hash (11) // Hash
#define MRBC_SSYM_Arduino (12) // Arduino
#define MRBC_SSYM_ESC_01 (13) // +
#define MRBC_SSYM_ESC_02 (14) // ===
#define MRBC_SSYM_ESC_03 (15) // <<
#define MRBC_SSYM_ESC_04 (16) // []
#define MRBC_SSYM_ESC_05 (17) // []=
#define MRBC_SSYM_ESC_LAMBDA (18) // ()
#define MRBC_SSYM_ESC_06 (19) // -
#define MRBC_SSYM_ESC_07 (20) // >>
