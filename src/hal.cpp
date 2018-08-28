#include <Arduino.h>
#include "console.h"
#include "hal.h"
#include "debug.h"


extern "C" void init_hal(){
#ifdef MMRUBY_DEBUG_ENABLE
  Serial.begin(SERIAL_FOR_STDIO_BAUDRATE);
#endif
}

extern "C" void hal_delay(int msec){
  delay(msec);
}

extern "C" void hal_write_string(char* text){
  Serial.print(text);
}

extern "C" int hal_write(int fd, const void *buf, int nbytes){
  char* t = (char*)buf;
  char tbuf[2];
  if(nbytes==1){ //for console_putchar()
    tbuf[0]=*t;
    tbuf[1]='\0';
    hal_write_string(tbuf);
    return nbytes;
  }
  if(nbytes<CONSOLE_BUFF_SIZE-1) t[nbytes]='\0';//TODO: double check
  hal_write_string(t);
  return nbytes;
}

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

