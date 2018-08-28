#ifndef __DEBUG_H_
#define __DEBUG_H_

#ifdef __cplusplus
//extern "C" {
#endif


enum mmruby_text{
  MMERROR_UNKNOWN,
  MMERROR_NOMEM,
};


#define MMRUBY_DEBUG_ENABLE

#ifdef MMRUBY_DEBUG_ENABLE
void debug_print(char* text);
void debug_println(char* text);
void debug_printb(unsigned char byte);

#define DEBUG_PRINT(x) Serial.print(x)
#define DEBUG_PRINTLN(x) Serial.println(x)
#define DEBUG_PRINTB(x) debug_printb(x)
#else
#define DEBUG_PRINT(x) 
#define DEBUG_PRINTLN(x) 
#define DEBUG_PRINTB(x)
#endif

#ifdef __cplusplus
//}
#endif

#endif
