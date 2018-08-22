#ifndef __MMRUBY_H_
#define __MMRUBY_H_

#include <avr/pgmspace.h>

#ifdef __cplusplus
extern "C" {
#endif

#define SERIAL_FOR_STDIO_BAUDRATE 115200

void mmruby_setup(const unsigned char* code);
void mmruby_run(void);

#ifdef __cplusplus
}
#endif

#endif

