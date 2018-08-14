#include <Arduino.h>
#include "debug.h"

extern "C" void debug_print(char* text){
  Serial.print(text);
}

extern "C" void debug_println(char* text){
  Serial.println(text);
}

