#include <Arduino.h>
#include "debug.h"

#ifdef MMRUBY_DEBUG_ENABLE

extern "C" void debug_print(char* text){
  Serial.print(text);
}

extern "C" void debug_println(char* text){
  Serial.println(text);
}

extern "C" void debug_printb(unsigned char byte){
  Serial.print(byte);
}

#endif

