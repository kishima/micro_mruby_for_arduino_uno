#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include "./vm.h"
#include "./static_symbols.h"
#include "./static_procs.h"
#include "../src/micro_vm.h"

#define MAX_IREP 256
#define MAX_OUTPUT_BUFF (1024*32)
static uint8_t  irep_output_buff[MAX_IREP][MAX_OUTPUT_BUFF];
static uint16_t irep_buff_length_list[MAX_IREP];

//Output file name
#define OUTPUT_FNAME1 "../src/code.h"
#define OUTPUT_FNAME2 "../src/symbol_ids.h"

//Symbol table(symbol)
#define MAX_SYMBOL (256)
#define MAX_SYMBOL_STR_LEN (256)
static char sym_tbl[MAX_SYMBOL][MAX_SYMBOL_STR_LEN];
static int sym_tbl_cnt = 0;

mrb_sym search_index(const char* str){
  int i;
  for(i=0;i<sym_tbl_cnt;i++){
    if(0 == strcmp(str, sym_tbl[i])){
      return i;
    }
  }
  return MAX_SYMBOL-1;
}

mrb_sym add_index(const char* str){
  if(sym_tbl_cnt>=MAX_SYMBOL-1){
    printf("symbol add_index error\n");
    return MAX_SYMBOL-1;
  }
  //symbol lenght must be less than MAX_SYMBOL_STR_LEN
  strcpy(sym_tbl[sym_tbl_cnt],str);
  sym_tbl_cnt++;
  return sym_tbl_cnt-1;
}

void init_symbol_tbl(){
  int i;
  add_index("");
  for(i=0;i<sizeof(static_symbols)/sizeof(char*)/2;i++){
    add_index(static_symbols[i*2]);
  }
}

static const char* check_offset(const char* str){
  //skip variable prefix
  int offset = 0;
  int c=0;
  while(1){
    char ch = *(str+c);
    if('\0' == ch ) break;
    if(ch=='@' || ch=='$' || ch==':' ){
      //
    }else{
      offset = c;
      break;
    }
    c++;
  }
  return str+offset;
}

mrb_sym str_to_symid(const char* str){
  const char* str_offset = check_offset(str);
  //printf("to_symid %s\n",str_offset);
  const mrb_sym sym_id = search_index(str_offset);
  if( sym_id < MAX_SYMBOL-1 ) return sym_id;
  return add_index( str_offset );
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
  fprintf(f,"const unsigned char %sirep_%03d[] PROGMEM= {\n",CODE_PREFIX,irep_id);
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
  //printf("code=\n");
  for(i=0;i<mirep->ilen * CODE_LEN;i++){
    buff[i] = irep->code[i];
    
    //if(i % CODE_LEN == 0 && i>0) printf("\n");
    //printf("%02x",buff[i]);
  }
  //printf("\n");
  return mirep->ilen * CODE_LEN;
}

uint16_t write_mirep_pools(uint8_t* buff, mrb_mirep* mirep,mrb_irep* irep){
  int i;
  for(i=0;i<mirep->plen;i++){
    buff[i] = irep->pool_head[i];
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
  case 0: { // IREP_TT_STRING
    obj->tt = MRB_TT_STRING;
    obj->str = (char*)p;
  } break;
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
  
  printf("irep length:header=%d,i(code)=%d,p(pool)=%d,r(ireps)=%d,s(symbol)=%d,total=%d\n",(int)sizeof(mrb_mirep),ilen,plen,rlen,slen,irep_buff_length_list[irep_id]);
  
  //output mirep header to buff
  p += write_mirep_struct(&buff[p],&mirep_header);

  //output code
  p += write_mirep_code(&buff[p],&mirep_header,irep);

  //output literal
  p += write_mirep_pools(&buff[p],&mirep_header,irep);

  //make symbol table
  int i=0;
  uint8_t* sym_p = irep->ptr_to_sym;
  sym_p += 4; //slen
  for(i=0;i<slen;i++){
    int s = bin_to_uint16(sym_p); sym_p += 2; //symbol length without NULL char
    mrb_sym sym_id = str_to_symid(sym_p);
    //printf("refer symbol(%s) id = %d\n",sym_p,sym_id);
    sym_p += s + 1;
    
    //output symbol_id
    buff[p] = sym_id;
    p += 1;
  }
  
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
  fprintf(f,"/* Irep table */\n");
  uint8_t irep_count=-1;

  analyze_irep_r(&irep_count,irep);

  printf("total irep=%d\n",irep_count+1);

  //output all irep to file
  int i;
  for(i=0;i<irep_count+1;i++){
    dump_trans_irep_bin(f,i,irep_output_buff[i],irep_buff_length_list[i]);
  }

  fprintf(f,"const unsigned char* const %sirep_table[] PROGMEM = {\n",CODE_PREFIX);
  for(i=0;i<irep_count+1;i++){
    char irep_name[256];
    sprintf(irep_name,"%sirep_%03d",CODE_PREFIX,i);
    fprintf(f,"  %s,\n",irep_name);
  }
  fprintf(f,"};\n");
  
}

void output_symbol_tbl(FILE* f){
  
  fprintf(f,"\n/* Symbol table */\n");
  fprintf(f,"#include \"%s\"\n\n","symbol_ids.h");
  fprintf(f,"const unsigned char %ssymbol_table_size PROGMEM = %d;\n",CODE_PREFIX,sym_tbl_cnt);
  
  int i;
  for(i=0;i<sym_tbl_cnt;i++){
    fprintf(f,"const char %ssymbol_ref%02x[] PROGMEM =\"%s\";\n",CODE_PREFIX,i,sym_tbl[i]);
  }

  fprintf(f,"\n");
  fprintf(f,"const char* const %ssymbol_table[] PROGMEM = {\n",CODE_PREFIX);
  //for symbols in ireps
  for(i=0;i<sym_tbl_cnt;i++){
    //fprintf(f,"  \"%s\",\n",sym_tbl[i]);
    fprintf(f,"  %ssymbol_ref%02x,\n",CODE_PREFIX,i);
  }
  fprintf(f,"};\n");
  fprintf(f,"\n");
 
}

void output_symbol_id_header(FILE* f){
  //symbol ids for basic classes
  fprintf(f,"#ifndef MRBC_SRC_SYMBOL_ID_H_\n");
  fprintf(f,"#define MRBC_SRC_SYMBOL_ID_H_\n");
  fprintf(f,"\n");
  int i;
  for(i=0;i<sizeof(static_symbols)/sizeof(char*)/2;i++){
    char* sname = strcmp(static_symbols[i*2+1],"") ? static_symbols[i*2+1] : static_symbols[i*2];
    fprintf(f,"#define MRBC_SSYM_%s (%d) // %s\n",sname,i+1,static_symbols[i*2]);
  }
  fprintf(f,"\n");
  fprintf(f,"#endif\n");
}

int class_name_to_tt(const char* class){
  if(0==strcmp(class,"Object"))return MRB_TT_OBJECT;
  if(0==strcmp(class,"String"))return MRB_TT_STRING;
  if(0==strcmp(class,"Nil"))return MRB_TT_NIL;
  if(0==strcmp(class,"Proc"))return MRB_TT_PROC;
  if(0==strcmp(class,"False"))return MRB_TT_FALSE;
  if(0==strcmp(class,"True"))return MRB_TT_TRUE;
  if(0==strcmp(class,"Fixnum"))return MRB_TT_FIXNUM;
  if(0==strcmp(class,"Symbol"))return MRB_TT_SYMBOL;
  if(0==strcmp(class,"Array"))return MRB_TT_ARRAY;
  if(0==strcmp(class,"Range"))return MRB_TT_RANGE;
  if(0==strcmp(class,"Hash"))return MRB_TT_HASH;
  return MRB_TT_EMPTY;
}

uint8_t find_class_table_top(const char* class){
  int proc_tbl_cnt = sizeof(static_procs)/sizeof(char*)/3;
  int i;
  for(i=0;i<proc_tbl_cnt;i++){
    if(0==strcmp(class,static_procs[i*3]))return i*3;
  }
  return 255;
}

void output_proc_switch_func(FILE* f, const char* class){
  uint8_t top = find_class_table_top(class);
  
  char* sym_name;
  //output c func
  fprintf(f,"mrb_func_t find_c_funcs_%s(mrb_sym sym_id){\n",class);
  fprintf(f,"  mrb_func_t func = 0;\n");
  if(top!=255){
    fprintf(f,"  switch(sym_id){\n");
    int i=0;
    while(0==strcmp(class,static_procs[top + i*3])){
      mrb_sym sym_id = str_to_symid(static_procs[top + i*3+1]);
      fprintf(f,"    case %d: func = %s; break;\n",sym_id,static_procs[top + i*3+2]);
      i++;
    }
    fprintf(f,"    default: break;\n");
    fprintf(f,"  }\n");
  }
  fprintf(f,"  return func;\n");
  fprintf(f,"}\n");
}

uint8_t find_class_table_top2(const char* class){
  int proc_tbl_cnt = sizeof(static_procs)/sizeof(char*)/3;
  int i;
  for(i=0;i<proc_tbl_cnt;i++){
    if(0==strcmp(class,static_procs[i*3]))return i;
  }
  return 255;
}

void output_procs_class(FILE* f,const char* class){
  fprintf(f,"\n");
  int proc_tbl_cnt = sizeof(static_procs)/sizeof(char*)/3;
  uint8_t top = find_class_table_top2(class);
  fprintf(f,"const uint8_t %sproc_table_%s[] PROGMEM = {\n",CODE_PREFIX,class);
  //{sym_id,method_id}
  if(top<255){
    int i=top;
    while(0==strcmp(class,static_procs[i*3])){
      fprintf(f,"  %d,%d,\n",str_to_symid(static_procs[i*3+1]), i+1);
      i++;
    }
  }
  fprintf(f,"  0\n");
  fprintf(f,"};\n");
}
void output_procs(FILE* f){
  int i;
  for(i=0;i<sizeof(basic_class_names)/sizeof(char*);i++){
    output_procs_class(f,basic_class_names[i]);
  }
}

void output_procs2(FILE* f){
  int i;
  fprintf(f,"inline mrb_func_t find_c_funcs_by_no(short no){\n");
  fprintf(f,"  mrb_func_t func=0;\n");
  fprintf(f,"  switch(no){\n");
  for(i=0;i<sizeof(static_procs)/sizeof(char*)/3 - 1;i++){
    fprintf(f,"    case %d: func = %s; break;\n",i+1,static_procs[i*3+2]);
  }
  fprintf(f,"    default: break;\n");
  fprintf(f,"  }\n");
  fprintf(f,"  return func;\n");
  fprintf(f,"}\n");
}

void output_proc_tbl(FILE* f){
  int proc_tbl_cnt = sizeof(static_procs)/sizeof(char*)/3;
  fprintf(f,"\n/* Proc table */\n");
  fprintf(f,"const unsigned char %sproc_table_size PROGMEM = %d;\n",CODE_PREFIX,proc_tbl_cnt-1);
  fprintf(f,"\n");
  output_procs(f);
  fprintf(f,"\n");
  output_procs2(f);
  fprintf(f,"\n");

  return;
#if 0
  fprintf(f,"\n/* C function for Proc table */\n");
  int i=0;
  for(i=0;i<sizeof(basic_class_names)/sizeof(char*);i++){
    output_proc_switch_func(f,basic_class_names[i]);
  }
  fprintf(f,"\n");
  fprintf(f,"mrb_func_t find_c_funcs(mrb_vtype tt,mrb_sym sym_id){\n");
  fprintf(f,"  mrb_func_t func = 0;\n");
  fprintf(f,"  switch(tt){\n");
  for(i=0;i<sizeof(basic_class_names)/sizeof(char*);i++){
    switch(class_name_to_tt(basic_class_names[i])){
    case MRB_TT_TRUE:
      fprintf(f,"    case MRB_TT_TRUE:func = find_c_funcs_%s(sym_id); break;\n",basic_class_names[i]);
      break;
    case MRB_TT_FALSE:
      fprintf(f,"    case MRB_TT_FALSE:func = find_c_funcs_%s(sym_id); break;\n",basic_class_names[i]);
      break;
    case MRB_TT_NIL:
      fprintf(f,"    case MRB_TT_NIL:func = find_c_funcs_%s(sym_id); break;\n",basic_class_names[i]);
      break;
    case MRB_TT_FIXNUM:
      fprintf(f,"    case MRB_TT_FIXNUM:func = find_c_funcs_%s(sym_id); break;\n",basic_class_names[i]);
      break;
    case MRB_TT_SYMBOL:
      fprintf(f,"    case MRB_TT_SYMBOL:func = find_c_funcs_%s(sym_id); break;\n",basic_class_names[i]);
      break;
    case MRB_TT_OBJECT:
      fprintf(f,"    case MRB_TT_OBJECT:func = find_c_funcs_%s(sym_id); break;\n",basic_class_names[i]);
      break;
    case MRB_TT_PROC:
      fprintf(f,"    case MRB_TT_PROC:func = find_c_funcs_%s(sym_id); break;\n",basic_class_names[i]);
      break;
    case MRB_TT_ARRAY:
      fprintf(f,"    case MRB_TT_ARRAY:func = find_c_funcs_%s(sym_id); break;\n",basic_class_names[i]);
      break;
    case MRB_TT_STRING:
      fprintf(f,"    case MRB_TT_STRING:func = find_c_funcs_%s(sym_id); break;\n",basic_class_names[i]);
      break;
    case MRB_TT_RANGE:
      fprintf(f,"    case MRB_TT_RANGE:func = find_c_funcs_%s(sym_id); break;\n",basic_class_names[i]);
      break;
    case MRB_TT_HASH:
      //fprintf(f,"    case MRB_TT_HASH:func = find_c_funcs_%s(sym_id); break;\n",basic_class_names[i]);
      break;
    default:
      break;
    }
  }
  fprintf(f,"    default: break;\n");
  fprintf(f,"  }\n");
  fprintf(f,"}\n");
#endif
}

void trans_code_mrb(mrb_vm* vm){
  printf("\n");
  printf("###### Trans mrb to C code ######\n");
  printf("\n");
  init_symbol_tbl();

  FILE* f = stdout;
  if ((f = fopen(OUTPUT_FNAME1, "w")) == NULL) {
    printf("connot open code.h\n");
    exit(0);
  }
  printf("** Open %s **\n",OUTPUT_FNAME1);
  //output Irep table
  analyze_irep(f,vm->irep);
  //output Proc table
  output_proc_tbl(f);
  //output Symbol table
  output_symbol_tbl(f);

  fclose(f);
  printf("** Close %s **\n",OUTPUT_FNAME1);

  if ((f = fopen(OUTPUT_FNAME2, "w")) == NULL) {
    printf("connot open code.h\n");
    exit(0);
  }
  printf("** Open %s **\n",OUTPUT_FNAME2);
  //output Symbol ID header
  output_symbol_id_header(f);

  fclose(f);
  printf("** Close %s **\n",OUTPUT_FNAME2);

}


