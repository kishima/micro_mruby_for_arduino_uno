#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include "./vm.h"
#include "../src/micro_vm.h"

#define MAX_IREP 256
static mrb_mirep mirep_list[MAX_IREP];

#define MAX_OUTPUT_BUFF (1024*32)
static uint8_t output_buff[MAX_IREP][MAX_OUTPUT_BUFF];
static uint16_t irep_buff_length_list[MAX_IREP];

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

void dump_trans_irep_bin(FILE* f, uint8_t irep_id, uint8_t* buff, uint16_t length){
  fprintf(f,"const char PROGMEM irep_%03d[] = {\n",irep_id);
  dump_bytes(f,buff,length);
  fprintf(f,"\n}\n");
}

uint16_t write_irep_struct(uint8_t* buff, mrb_mirep* mirep){
  //struct
  buff[0] = mirep->nlocals;
  buff[1] = mirep->nregs;
  buff[2] = mirep->rlen;
  buff[3] = mirep->ilen;
  buff[4] = mirep->plen;
  uint16_to_bin(mirep->sym_pos,&buff[5]);
  //irep list
  //code
  //pools
  //symbol
  return 7;
}

void analyze_irep_r(FILE* f, int* irep_count, mrb_irep* irep){
  *irep_count = *irep_count + 1;
  printf("analyze_irep_r irep=%d\n",*irep_count);

  output_irep_header(f,irep);

  int i=0;
  for(i=0;i<irep->rlen;i++){
    analyze_irep_r(irep_count,irep->reps[i]);
  }
}

void make_mirep(mrb_irep* irep,mrb_mirep* mirep){
  mirep->nlocals = irep->nlocals;
  mirep->nregs = irep->nregs;
  mirep->rlen = irep->rlen;
  mirep->ilen = irep->ilen;
  mirep->plen = irep->plen;
}

uint8_t* get_irep_p(uint8_t irep_id){
  return output_buff[irep_id];
}


void analyze_irep(FILE* f,mrb_irep* irep){
  uint8_t irep_count=0;
  uint16_t p=0;
  printf("analyze_irep irep=%d\n",irep_count);

  //
  mrb_irep* mirep = &mirep_list[irep_count];
  uint8_t* buff = output_buff[irep_count];
  
  //make header
  make_mirep(irep,mirep);
  
  //output to buff
  p += write_irep_struct(output_buff,mirep);
  p += mirep->rlen; //length of child irep
  p += mirep->ilen; //length of code
  p += mirep->pool_total_len; //length of pools
  p += mirep->sym_total_len; //length of sym_total_len
  
  
  int i=0;
  for(i=0;i<irep->rlen;i++){
    analyze_irep_r(f,&irep_count,irep->reps[i]);
    //set child irep id
    mirep
  }
  
  
  printf("total irep=%d\n",irep_count+1);

  //output all to file
  for(i=0;i<irep_count+1;i++){
    dump_trans_irep_bin(f,i,output_buff[i],irep_buff_length_list[i]);
  }
}

void trans_code_mrb(mrb_vm* vm){
  //dump head
  dump_array_bytes(stdout,"const unsigned char PROGMEM code[]={\n",
                   (uint8_t*)(vm->irep),sizeof(mrb_irep));

  //count ireps
  analyze_irep(stdout,vm->irep);
  //make an irep list

  //all_irep[]={ *** };
  //irep_list[]={ 0 , 20, 40 , *** }
}


