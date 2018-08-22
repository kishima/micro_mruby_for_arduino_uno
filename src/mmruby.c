#include "mmruby.h"
#include "vm.h"
#include "debug.h"

void mmruby_setup(const unsigned char* code){
  init_vm();

  //load bytecode
  
  for(unsigned char i=0;i<10;i++){
    DEBUG_PRINTB(i);
    DEBUG_PRINT(":");
    DEBUG_PRINTB(pgm_read_byte(&(code[i])));
    DEBUG_PRINT("\n");
  }
  
}


void mmruby_run(void){
  run_vm();
}


