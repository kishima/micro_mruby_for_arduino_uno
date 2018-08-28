#include <Arduino.h>
#include "console.h"
#include "hal.h"
#include "debug.h"


void init_hal(){
#ifdef MMRUBY_DEBUG_ENABLE
  Serial.begin(SERIAL_FOR_STDIO_BAUDRATE);
#endif
}

void hal_delay(int msec){
  delay(msec);
}

void hal_write_string(char* text){
  Serial.print(text);
}

int hal_write(int fd, const void *buf, int nbytes){
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

void debug_print(char* text){
  Serial.print(text);
}

void debug_println(char* text){
  Serial.println(text);
}

void debug_printb(unsigned char byte){
  Serial.print(byte);
}

#endif

