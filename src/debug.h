#ifndef __DEBUG_H_
#define __DEBUG_H_

#include <Arduino.h>

enum mmruby_text{
  MMERROR_UNKNOWN,
  MMERROR_NOMEM,
};

#define MMRUBY_DEBUG_ENABLE

#ifdef MMRUBY_DEBUG_ENABLE
void debug_print(char* text);
void debug_println(char* text);
void debug_printb(unsigned char byte);

#include <avr/pgmspace.h>
#define DEBUG_FPRINT(x) Serial.print(F(x))
#define DEBUG_FPRINTLN(x) Serial.println(F(x))
#define DEBUG_PRINT(x) Serial.print(x)
#define DEBUG_PRINTLN(x) Serial.println(x)
#else
#define DEBUG_FPRINT(x) 
#define DEBUG_FPRINTLN(x) 
#define DEBUG_PRINT(x) 
#define DEBUG_PRINTLN(x) 
#endif


#endif
