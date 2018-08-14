#ifndef __DEBUG_H_
#define __DEBUG_H_

#ifdef __cplusplus
extern "C" {
#endif

#define MMRUBY_DEBUG_ENABLE

#ifdef MMRUBY_DEBUG_ENABLE
#define DEBUG_PRINT(x) debug_print(x)
#define DEBUG_PRINTLN(x) debug_println(x)
#else
#define DEBUG_PRINT(x) 
#define DEBUG_PRINTLN(x) 
#endif

#ifdef __cplusplus
}
#endif

#endif
