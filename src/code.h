/* Irep table */
const unsigned char mmruby_code_irep_000[] PROGMEM= {
0x00,0x04,0x0E,0x01,0x00,0x80,0x00,0x06,0x01,0x00,0x00,0x3D,0x00,0x80,0x00,0xA0,
0x00,0x00,0x00,0x4A,0x00,0x00,0x0B,0x48,0x65,0x6C,0x6C,0x6F,0x20,0x57,0x6F,0x72,
0x6C,0x64,0x0B,
};
const unsigned char* const mmruby_code_irep_table[] PROGMEM = {
  mmruby_code_irep_000,
};

/* Symbol table */
const unsigned char mmruby_code_symbol_table_size PROGMEM = 29;
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
