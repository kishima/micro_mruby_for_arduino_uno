#include <stdint.h>
#include "debug.h"
#include <avr/pgmspace.h>

const char string01[] PROGMEM = { "UNKNOWN ERROR" };
const char string02[] PROGMEM = { "NOMEM ERROR" };

const char* string_list[] PROGMEM = {
  string01,
  string02
};

void output_string(byte no){
  char* string = (char*)pgm_read_word(&(string_list[no]));
  int i = 0;
  char ch[2];
  ch[0] = pgm_read_byte(&(string[0]));
  ch[1] = '\0';
  while(ch != '\0'){
    ch[0] = pgm_read_byte(&(string + i))
    debug_print(ch);
    i++;
  };
}
