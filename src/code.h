/* Irep table */
const unsigned char mmruby_code_irep_000[] PROGMEM= {
0x00,0x04,0x0E,0x01,0x00,0x80,0x00,0x06,0x01,0x00,0x00,0x3D,0x00,0x80,0x00,0xA0,
0x00,0x00,0x00,0x4A,0x00,0x00,0x0B,0x48,0x65,0x6C,0x6C,0x6F,0x20,0x57,0x6F,0x72,
0x6C,0x64,0x0C,
};
const unsigned char* const mmruby_code_irep_table[] PROGMEM = {
  mmruby_code_irep_000,
};

/* Proc table */
const unsigned char mmruby_code_proc_table_size PROGMEM = 7;


const uint8_t mmruby_code_proc_table_Object[] PROGMEM = {
  12,1,
  30,2,
  0
};

const uint8_t mmruby_code_proc_table_Proc[] PROGMEM = {
  0
};

const uint8_t mmruby_code_proc_table_False[] PROGMEM = {
  0
};

const uint8_t mmruby_code_proc_table_True[] PROGMEM = {
  0
};

const uint8_t mmruby_code_proc_table_Nil[] PROGMEM = {
  0
};

const uint8_t mmruby_code_proc_table_Array[] PROGMEM = {
  0
};

const uint8_t mmruby_code_proc_table_Fixnum[] PROGMEM = {
  0
};

const uint8_t mmruby_code_proc_table_String[] PROGMEM = {
  13,3,
  14,4,
  15,5,
  30,6,
  0
};

const uint8_t mmruby_code_proc_table_Symbol[] PROGMEM = {
  0
};

const uint8_t mmruby_code_proc_table_Range[] PROGMEM = {
  0
};

/* C function for Proc table */
mrb_func_t find_c_funcs_Object(mrb_sym sym_id){
  mrb_func_t func = 0;
  switch(sym_id){
    case 12: func = c_puts; break;
    case 30: func = c_puts; break;
    default: break;
  }
  return func;
}
mrb_func_t find_c_funcs_Proc(mrb_sym sym_id){
  mrb_func_t func = 0;
  return func;
}
mrb_func_t find_c_funcs_False(mrb_sym sym_id){
  mrb_func_t func = 0;
  return func;
}
mrb_func_t find_c_funcs_True(mrb_sym sym_id){
  mrb_func_t func = 0;
  return func;
}
mrb_func_t find_c_funcs_Nil(mrb_sym sym_id){
  mrb_func_t func = 0;
  return func;
}
mrb_func_t find_c_funcs_Array(mrb_sym sym_id){
  mrb_func_t func = 0;
  return func;
}
mrb_func_t find_c_funcs_Fixnum(mrb_sym sym_id){
  mrb_func_t func = 0;
  return func;
}
mrb_func_t find_c_funcs_String(mrb_sym sym_id){
  mrb_func_t func = 0;
  switch(sym_id){
    case 13: func = c_string_add; break;
    case 14: func = c_string_eql; break;
    case 15: func = c_string_size; break;
    case 30: func = c_string_size; break;
    default: break;
  }
  return func;
}
mrb_func_t find_c_funcs_Symbol(mrb_sym sym_id){
  mrb_func_t func = 0;
  return func;
}
mrb_func_t find_c_funcs_Range(mrb_sym sym_id){
  mrb_func_t func = 0;
  return func;
}

mrb_func_t find_c_funcs(mrb_vtype tt,mrb_sym sym_id){
  mrb_func_t func = 0;
  switch(tt){
    case MRB_TT_OBJECT:func = find_c_funcs_Object(sym_id); break;
    case MRB_TT_PROC:func = find_c_funcs_Proc(sym_id); break;
    case MRB_TT_FALSE:func = find_c_funcs_False(sym_id); break;
    case MRB_TT_TRUE:func = find_c_funcs_True(sym_id); break;
    case MRB_TT_NIL:func = find_c_funcs_Nil(sym_id); break;
    case MRB_TT_ARRAY:func = find_c_funcs_Array(sym_id); break;
    case MRB_TT_FIXNUM:func = find_c_funcs_Fixnum(sym_id); break;
    case MRB_TT_STRING:func = find_c_funcs_String(sym_id); break;
    case MRB_TT_SYMBOL:func = find_c_funcs_Symbol(sym_id); break;
    case MRB_TT_RANGE:func = find_c_funcs_Range(sym_id); break;
    default: break;
  }
}

/* Symbol table */
const unsigned char mmruby_code_symbol_table_size PROGMEM = 31;
const char* const mmruby_code_symbol_table[] PROGMEM = {
  "",
  "Object",
  "String",
  "Nil",
  "Proc",
  "False",
  "True",
  "Fixnum",
  "Symbol",
  "Array",
  "Range",
  "Hash",
  "puts",
  "+",
  "===",
  "size",
  "length",
  "to_i",
  "to_s",
  "<<",
  "[]",
  "[]=",
  "chomp",
  "chomp!",
  "dup",
  "index",
  "ord",
  "to_sym",
  "intern",
  "sprintf",
  "test",
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
#define MRBC_SSYM_puts (12) // puts
#define MRBC_SSYM_ESC_01 (13) // +
#define MRBC_SSYM_ESC_02 (14) // ===
#define MRBC_SSYM_size (15) // size
#define MRBC_SSYM_length (16) // length
#define MRBC_SSYM_to_i (17) // to_i
#define MRBC_SSYM_to_s (18) // to_s
#define MRBC_SSYM_ESC_03 (19) // <<
#define MRBC_SSYM_ESC_04 (20) // []
#define MRBC_SSYM_ESC_05 (21) // []=
#define MRBC_SSYM_chomp (22) // chomp
#define MRBC_SSYM_ESC_06 (23) // chomp!
#define MRBC_SSYM_dup (24) // dup
#define MRBC_SSYM_index (25) // index
#define MRBC_SSYM_ord (26) // ord
#define MRBC_SSYM_to_sym (27) // to_sym
#define MRBC_SSYM_intern (28) // intern
#define MRBC_SSYM_sprintf (29) // sprintf
