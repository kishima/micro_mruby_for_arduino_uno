#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <vm.h>


void dump_byte(FILE* f, uint8_t b){
  fprintf(f,"0x%02X",b);
}
void dump_bytes(FILE* f, uint8_t* bp, int length){
  int i=0;
  for(i=0;i<length;i++){
    dump_byte(f,bp[i]);
    fprintf(f,",");
    if(i%16==15) fprintf(f,"\n");
  }
  
}
void dump_array_bytes(FILE* f, char* name, uint8_t* bp, int length){
  fprintf(f,"%s",name);
  dump_bytes(f,bp,length);
  fprintf(f,"\n}\n");
}

void trans_code_mrb(mrb_vm* vm){
  //dump head
  dump_array_bytes(stdout,"const unsigned char PROGMEM code[]={\n",
                   (uint8_t*)(vm->irep),sizeof(mrb_irep));

  //count ireps

  //make an irep list

  //all_irep[]={ *** };
  //irep_list[]={ 0 , 20, 40 , *** }
}


