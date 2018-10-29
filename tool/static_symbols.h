#include "../src/ext_conf.h"

static char* static_symbols[] = {
//Class
   "Object","",
   "String","",
   "Nil","",
   "Proc","",
   "False","",
   "True","",
   "Fixnum","",
   "Symbol","",
   "Array","",
   "Range","",
   "Hash","",
   "Arduino","",
#ifdef USE_RGB_LCD
   "RGB_LCD","",
#endif
  //methods and others
   "true","",
   "false","",
   "to_s","",
   "INPUT","",
   "OUTPUT","",
   "INPUT_PULLUP","",
   "HIGH","",
   "LOW","",
   "+","ESC_01",
   "===","ESC_02",
   "<<","ESC_03",
   "[]","ESC_04",
   "[]=","ESC_05",
   "()","ESC_LAMBDA",
   "-","ESC_06",
   ">>","ESC_07",
};
