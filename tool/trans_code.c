#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include "./vm.h"
#include "../src/micro_vm.h"

#define CODE_PREFIX "mmruby_code_"

#define CODE_LEN 4

#define MAX_IREP 256
#define MAX_OUTPUT_BUFF (1024*32)
static uint8_t  irep_output_buff[MAX_IREP][MAX_OUTPUT_BUFF];
static uint16_t irep_buff_length_list[MAX_IREP];


//Symbol table(symbol)
#define MAX_SYMBOL (256)
#define MAX_SYMBOL_STR_LEN (256)
static char sym_tbl[MAX_SYMBOL][MAX_SYMBOL_STR_LEN];
static int sym_tbl_cnt = 0;

uint8_t search_index(char* str){
  int i;
  for(i=0;i<sym_tbl_cnt;i++){
    if(0 == strcmp(str, sym_tbl[i])){
      return i;
    }
  }
  return MAX_SYMBOL-1;
}

uint8_t add_index(char* str){
  if(sym_tbl_cnt>=MAX_SYMBOL-1){
    printf("symbol add_index error\n");
    return MAX_SYMBOL-1;
  }
  //symbol lenght must be less than MAX_SYMBOL_STR_LEN
  strcpy(sym_tbl[sym_tbl_cnt],str);
  sym_tbl_cnt++;
  return sym_tbl_cnt-1;
}

uint8_t str_to_symid(char* str){
  uint8_t sym_id = search_index(str);
  if( sym_id < MAX_SYMBOL-1 ) return (uint8_t)sym_id;
  return add_index( str );
}

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
  fprintf(f,"\n};\n");
}

void dump_trans_irep_bin(FILE* f, uint8_t irep_id, uint8_t* buff, uint16_t length){
  fprintf(f,"const char %sirep_%03d[] PROGMEM= {\n",CODE_PREFIX,irep_id);
  dump_bytes(f,buff,length);
  fprintf(f,"\n};\n");
}

//----------------------------------------

uint16_t write_mirep_struct(uint8_t* buff, mrb_mirep* mirep){
  //struct
  buff[0] = mirep->rlen;
  buff[1] = mirep->ilen;
  buff[2] = mirep->plen;
  buff[3] = mirep->slen;
  return 4;
}

uint16_t write_mirep_code(uint8_t* buff, mrb_mirep* mirep,mrb_irep* irep){
  int i;
  for(i=0;i<mirep->ilen * CODE_LEN;i++){
    buff[i] = irep->code[i];
  }
  return mirep->ilen * CODE_LEN;
}

uint16_t write_mirep_pools(uint8_t* buff, mrb_mirep* mirep,mrb_irep* irep){
  int i;
  for(i=0;i<mirep->plen;i++){
    buff[i] = ((uint8_t*)irep->pools)[i];
  }
  return mirep->plen;
}

void make_mirep(mrb_irep* irep,mrb_mirep* mirep){
  mirep->rlen = irep->rlen;
  mirep->ilen = irep->ilen;
  mirep->plen = irep->pool_total_len;
  mirep->slen = bin_to_uint32(irep->ptr_to_sym);
}


int make_mrb_obj(uint8_t* p, mrb_object *obj){
  int tt = *p++;
  int obj_size = bin_to_uint16(p);	p += 2;
  //mrb_object *obj = mrbc_obj_alloc(0, MRB_TT_EMPTY);
  if( obj == NULL ) {
    return 0;
  }
  switch( tt ) {
#if MRBC_USE_STRING
  case 0: { // IREP_TT_STRING
    obj->tt = MRB_TT_STRING;
    obj->str = (char*)p;
  } break;
#endif
  case 1: { // IREP_TT_FIXNUM
    char buf[obj_size+1];
    memcpy(buf, p, obj_size);
    buf[obj_size] = '\0';
    obj->tt = MRB_TT_FIXNUM;
    obj->i = atol(buf);
  } break;
  default:
    printf("Literal object not supported\n");
    break;
  }
  return obj_size;
}

uint8_t analyze_irep_r(uint8_t* irep_count, mrb_irep* irep){
  *irep_count = *irep_count + 1;
  uint8_t irep_id = *irep_count;
  printf("analyze_irep_r irep=%d\n",irep_id);

  uint16_t p=0;
  uint16_t mp=0;

  mrb_mirep mirep_header;
  uint8_t* buff = irep_output_buff[irep_id];

  //make header
  make_mirep(irep,&mirep_header);
  
  int rlen = mirep_header.rlen;
  int ilen = mirep_header.ilen;
  int plen = mirep_header.plen;
  int slen = mirep_header.slen;
  irep_buff_length_list[irep_id] = sizeof(mrb_mirep)+ ilen*CODE_LEN + plen + rlen +slen;
  
  printf("irep length:header=%d,i=%d,p=%d,r=%d,s=%d,total=%d\n",(int)sizeof(mrb_mirep),ilen,plen,rlen,slen,irep_buff_length_list[irep_id]);
  
  //output mirep header to buff
  p += write_mirep_struct(&buff[p],&mirep_header);
  //printf("p=%d\n",p);

  //output code
  p += write_mirep_code(&buff[p],&mirep_header,irep);
  //printf("p=%d\n",p);

  //output literal
  p += write_mirep_pools(&buff[p],&mirep_header,irep);
  //printf("p=%d\n",p);

  //make symbol table
  int i=0;
  uint8_t* sym_p = irep->ptr_to_sym;
  sym_p += 4; //slen
  for(i=0;i<slen;i++){
    int s = bin_to_uint16(sym_p); sym_p += 2; //symbol length without NULL char
    uint8_t sym_id = str_to_symid(sym_p);
    printf("new symbol[%d]=%s\n",sym_id,sym_p);
    sym_p += s + 1;
    
    //output symbol_id
    buff[p] = sym_id;
    p += 1;
  }
  //printf("p=%d\n",p);
  
  //output child irep id
  for(i=0;i<irep->rlen;i++){
    uint8_t child_irep_id = analyze_irep_r(irep_count,irep->reps[i]);
    buff[p] = child_irep_id;
    p += 1;
  }
  printf("check irep size %d/%d\n",p,irep_buff_length_list[irep_id]);
  return irep_id;
}


void analyze_irep(FILE* f,mrb_irep* irep){
  uint8_t irep_count=-1;

  analyze_irep_r(&irep_count,irep);

  printf("total irep=%d\n",irep_count+1);

  //output all irep to file
  int i;
  for(i=0;i<irep_count+1;i++){
    dump_trans_irep_bin(f,i,irep_output_buff[i],irep_buff_length_list[i]);
  }

  fprintf(f,"char* const %sirep_table[] PROGMEM = {\n",CODE_PREFIX);
  for(i=0;i<irep_count+1;i++){
    char irep_name[256];
    sprintf(irep_name,"%sirep_%03d",CODE_PREFIX,i);
    fprintf(f,"  %s,\n",irep_name);
  }
  fprintf(f,"};\n");
  
}

void output_symbol_tbl(FILE* f){
  
  fprintf(f,"const unsigned char %ssymbol_table_size PROGMEM = %d;\n",CODE_PREFIX,sym_tbl_cnt);
  
  fprintf(f,"char* const %ssymbol_table[] PROGMEM = {\n",CODE_PREFIX);
  int i;
  for(i=0;i<sym_tbl_cnt;i++){
    fprintf(f,"  \"%s\",\n",sym_tbl[i]);
  }
  fprintf(f,"};\n");
}

void trans_code_mrb(mrb_vm* vm){
  //open file
  FILE* f = stdout;
  
  //analyze and output
  analyze_irep(f,vm->irep);
  
  output_symbol_tbl(f);


  //close
}


