#ifndef __MMRUBY_VM_H_
#define __MMRUBY_VM_H_

#define MMRUBY_DEBUG_ENABLE

#ifdef MMRUBY_DEBUG_ENABLE
#define DEBUG_PRINT(x) Serial.print(x)
#define DEBUG_PRINTLN(x) Serial.println(x)
#else
#define DEBUG_PRINT(x) 
#define DEBUG_PRINTLN(x) 
#endif

#endif
