#include <stdint.h>
#include <string.h>
#include "micro_vm.h"
#include "opcode.h"
#include "console.h"
#include "symbol.h"
#include "debug.h"

#include <Arduino.h>
#include <avr/pgmspace.h>

#include "code.h"

#include "avr_access.h"

inline static void memcpy_pgm2ram(uint8_t* buff, short pgm_p, uint16_t len){
  int i=0;
  for(i=0;i<len;i++){
    buff[i] = pgm_read_byte_near( pgm_p+i );
  }
}

inline static void strcpy_pgm2ram(uint8_t* buff, short pgm_p){
  int i=0;
  for(i=0;i<MAX_SYMBOL_LEN;i++){
    char c = pgm_read_byte_near( pgm_p+i );
    buff[i] = c;
    if(c=='\0') break;
  }
}

uint32_t read_bytecode(uint8_t irep_id, uint8_t pc){
  return pgm_read_dword_near( pgm_read_word_near(&mmruby_code_irep_table[irep_id]) + MIREP_HEADER_SIZE + pc*CODE_LEN );
}

void get_irep_pool(uint8_t* buff, uint16_t* obj_size, uint8_t irep_id, uint8_t no){
  short irep_addr = pgm_read_word_near(&mmruby_code_irep_table[irep_id]);
  uint8_t code_len = pgm_read_byte_near( irep_addr + MIREP_OFFSET_ILEN ) * CODE_LEN;
  uint8_t plen = pgm_read_byte_near( irep_addr + MIREP_OFFSET_PLEN );
  if(no > plen-1){
    DEBUG_FPRINTLN("ERROR no>plen");
    return;
  }
  int i;
  uint8_t p = 0;
  uint8_t tt = 0;
  short obj_p = 0;
  for(i=0;i<=no;i++){ //no begins from 0
    tt = pgm_read_byte_near( irep_addr + MIREP_HEADER_SIZE + code_len + p);
    p++;
    uint16_t bin = pgm_read_word_near( irep_addr + MIREP_HEADER_SIZE + code_len + p);
    *obj_size = bin_to_uint16( &bin );
    p += 2;
    obj_p = p;
    p += *obj_size;
  }
  memcpy_pgm2ram(buff, irep_addr + MIREP_HEADER_SIZE + code_len + obj_p,*obj_size);
  return;
}

//read_symbol
uint8_t get_irep_symbol_id(uint8_t irep_id, uint8_t no){
  short irep_addr = pgm_read_word_near(&mmruby_code_irep_table[irep_id]);
  uint8_t code_len = pgm_read_byte_near( irep_addr + MIREP_OFFSET_ILEN ) * CODE_LEN;
  uint8_t plen = pgm_read_byte_near( irep_addr + MIREP_OFFSET_PLEN );
  return pgm_read_byte_near( irep_addr + MIREP_HEADER_SIZE + code_len + plen + no);
}

//

mrb_sym search_index_static(const char *str){
  char buff[MAX_SYMBOL_LEN];
  uint8_t max = pgm_read_byte_near(&mmruby_code_symbol_table_size);
  int i;
  for(i=0;i<max;i++){
    short addr = pgm_read_word_near(&mmruby_code_symbol_table[i]);
    strcpy_pgm2ram(buff, addr);
    if(0 == strcmp(str, buff)){
      if(i==0)return INVALID_SYMBOL; //str is null char
      return i;
    }
  }
  return INVALID_SYMBOL;
}
