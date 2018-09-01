/* Irep table */
const unsigned char mmruby_code_irep_000[] PROGMEM= {
0x01,0x0A,0x10,0x02,0x00,0x80,0x00,0x48,0x01,0x00,0x00,0xC0,0x00,0x80,0x00,0x46,
0x00,0x80,0x00,0x06,0x01,0x00,0x00,0x3D,0x00,0x80,0x40,0xA0,0x00,0x80,0x00,0x06,
0x01,0x00,0x00,0xBD,0x00,0x80,0x00,0xA0,0x00,0x00,0x00,0x4A,0x00,0x00,0x04,0x54,
0x65,0x73,0x74,0x00,0x00,0x06,0x48,0x65,0x6C,0x6C,0x6F,0x21,0x1E,0x0C,0x01,
};
const unsigned char mmruby_code_irep_001[] PROGMEM= {
0x00,0x05,0x00,0x01,0x02,0x00,0x00,0x26,0x01,0x80,0x00,0x06,0x02,0x00,0x40,0x01,
0x01,0x80,0x00,0xA0,0x01,0x80,0x00,0x29,0x0C,
};
const unsigned char* const mmruby_code_irep_table[] PROGMEM = {
  mmruby_code_irep_000,
  mmruby_code_irep_001,
};

/* Proc table */
const unsigned char mmruby_code_proc_table_size PROGMEM = 6;


const uint8_t mmruby_code_proc_table_Object[] PROGMEM = {
  12,1,
  31,2,
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
  31,6,
  0
};

const uint8_t mmruby_code_proc_table_Symbol[] PROGMEM = {
  0
};

const uint8_t mmruby_code_proc_table_Range[] PROGMEM = {
  0
};

inline mrb_func_t find_c_funcs_by_no(short no){
  mrb_func_t func=0;
  switch(no){
    case 1: func = c_puts; break;
    case 2: func = c_puts; break;
    case 3: func = c_string_add; break;
    case 4: func = c_string_eql; break;
    case 5: func = c_string_size; break;
    case 6: func = c_string_size; break;
    default: break;
  }
  return func;
}


/* Symbol table */
const unsigned char mmruby_code_symbol_table_size PROGMEM = 32;
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
const char mmruby_code_symbol_ref0c[] PROGMEM ="puts";
const char mmruby_code_symbol_ref0d[] PROGMEM ="+";
const char mmruby_code_symbol_ref0e[] PROGMEM ="===";
const char mmruby_code_symbol_ref0f[] PROGMEM ="size";
const char mmruby_code_symbol_ref10[] PROGMEM ="length";
const char mmruby_code_symbol_ref11[] PROGMEM ="to_i";
const char mmruby_code_symbol_ref12[] PROGMEM ="to_s";
const char mmruby_code_symbol_ref13[] PROGMEM ="<<";
const char mmruby_code_symbol_ref14[] PROGMEM ="[]";
const char mmruby_code_symbol_ref15[] PROGMEM ="[]=";
const char mmruby_code_symbol_ref16[] PROGMEM ="chomp";
const char mmruby_code_symbol_ref17[] PROGMEM ="chomp!";
const char mmruby_code_symbol_ref18[] PROGMEM ="dup";
const char mmruby_code_symbol_ref19[] PROGMEM ="index";
const char mmruby_code_symbol_ref1a[] PROGMEM ="ord";
const char mmruby_code_symbol_ref1b[] PROGMEM ="to_sym";
const char mmruby_code_symbol_ref1c[] PROGMEM ="intern";
const char mmruby_code_symbol_ref1d[] PROGMEM ="sprintf";
const char mmruby_code_symbol_ref1e[] PROGMEM ="method";
const char mmruby_code_symbol_ref1f[] PROGMEM ="test";

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
