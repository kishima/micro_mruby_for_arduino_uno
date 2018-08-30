#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <assert.h>
#include "./vm.h"
#include "../src/micro_vm.h"


static void show_opcode(uint32_t code){
  uint8_t opcode = GET_OPCODE(code);
  char* name = "UNKNOWN";
  switch( opcode ) {
  case OP_NOP:        name = "OP_NOP"; break;
  case OP_MOVE:       name = "OP_MOVE"; break;
  case OP_LOADL:      name = "OP_LOADL"; break;
  case OP_LOADI:      name = "OP_LOADI"; break;
  case OP_LOADSYM:    name = "OP_LOADSYM"; break;
  case OP_LOADNIL:    name = "OP_LOADNIL"; break;
  case OP_LOADSELF:   name = "OP_LOADSELF"; break;
  case OP_LOADT:      name = "OP_LOADT"; break;
  case OP_LOADF:      name = "OP_LOADF"; break;
  case OP_GETGLOBAL:  name = "OP_GETGLOBAL"; break;
  case OP_SETGLOBAL:  name = "OP_SETGLOBAL"; break;
  case OP_GETIV:      name = "OP_GETIV"; break;
  case OP_SETIV:      name = "OP_SETIV"; break;
  case OP_GETCONST:   name = "OP_GETCONST"; break;
  case OP_SETCONST:   name = "OP_SETCONST"; break;
  case OP_GETUPVAR:   name = "OP_GETUPVAR"; break;
  case OP_SETUPVAR:   name = "OP_SETUPVAR"; break;
  case OP_JMP:        name = "OP_JMP"; break;
  case OP_JMPIF:      name = "OP_JMPIF"; break;
  case OP_JMPNOT:     name = "OP_JMPNOT"; break;
  case OP_SEND:       name = "OP_SEND"; break;
  case OP_SENDB:      name = "OP_SENDB"; break;  // reuse
  case OP_CALL:       name = "OP_CALL"; break;
  case OP_ENTER:      name = "OP_ENTER"; break;
  case OP_RETURN:     name = "OP_RETURN"; break;
  case OP_BLKPUSH:    name = "OP_BLKPUSH"; break;
  case OP_ADD:        name = "OP_ADD"; break;
  case OP_ADDI:       name = "OP_ADDI"; break;
  case OP_SUB:        name = "OP_SUB"; break;
  case OP_SUBI:       name = "OP_SUBI"; break;
  case OP_MUL:        name = "OP_MUL"; break;
  case OP_DIV:        name = "OP_DIV"; break;
  case OP_EQ:         name = "OP_EQ"; break;
  case OP_LT:         name = "OP_LT"; break;
  case OP_LE:         name = "OP_LE"; break;
  case OP_GT:         name = "OP_GT"; break;
  case OP_GE:         name = "OP_GE"; break;
  case OP_ARRAY:      name = "OP_ARRAY"; break;
  case OP_STRING:     name = "OP_STRING"; break;
  case OP_STRCAT:     name = "OP_STRCAT"; break;
  case OP_HASH:       name = "OP_HASH"; break;
  case OP_LAMBDA:     name = "OP_LAMBDA";break;
  case OP_RANGE:      name = "OP_RANGE"; break;
  case OP_CLASS:      name = "OP_CLASS"; break;
  case OP_EXEC:       name = "OP_EXEC"; break;
  case OP_METHOD:     name = "OP_METHOD"; break;
  case OP_TCLASS:     name = "OP_TCLASS"; break;
  case OP_STOP:       name = "OP_STOP"; break;
  case OP_ABORT:      name = "OP_ABORT"; break;  // reuse
  default:
    break;
  }
  printf(" > %08x (%s)\n",code,name);
}


static int load_header(mrb_vm *vm, const uint8_t **pos)
{
  const uint8_t *p = *pos;

  if( memcmp(p, "RITE0004", 8) != 0 ) {
    vm->error_code = LOAD_FILE_HEADER_ERROR_VERSION;
    return -1;
  }

  /* Ignore CRC */

  /* Ignore size */

  if( memcmp(p + 14, "MATZ", 4) != 0 ) {
    vm->error_code = LOAD_FILE_HEADER_ERROR_MATZ;
    return -1;
  }
  if( memcmp(p + 18, "0000", 4) != 0 ) {
    vm->error_code = LOAD_FILE_HEADER_ERROR_VERSION;
    return -1;
  }

  *pos += 22;
  return 0;
}

static mrb_irep * load_irep_1(mrb_vm *vm, const uint8_t **pos)
{
  const uint8_t *p = *pos + 4;			// skip record size

  // new irep
  mrb_irep *irep = malloc(sizeof(mrb_irep));
  if( irep == NULL ) {
    vm->error_code = LOAD_FILE_IREP_ERROR_ALLOCATION;
    return NULL;
  }

  // nlocals,nregs,rlen
  irep->nlocals = bin_to_uint16(p);	p += 2;
  irep->nregs = bin_to_uint16(p);	p += 2;
  irep->rlen = bin_to_uint16(p);	p += 2;
  irep->ilen = bin_to_uint32(p);	p += 4;
  
  printf("irep->nlocals=%d\n",irep->nlocals);
  printf("irep->nregs=%d\n",irep->nregs);
  printf("irep->rlen=%d\n",irep->rlen);
  printf("irep->ilen=%d\n",irep->ilen);
  
  // padding
  p += (vm->mrb - p) & 0x03;

  // allocate memory for child irep's pointers
  if( irep->rlen ) {
    irep->reps = (mrb_irep **)malloc(sizeof(mrb_irep *) * irep->rlen);
    printf("irep->reps[0]=%p\n",irep->reps[0]);
    if( irep->reps == NULL ) {
      vm->error_code = LOAD_FILE_IREP_ERROR_ALLOCATION;
      return NULL;
    }
  }

  // ISEQ (code) BLOCK
  irep->code = (uint8_t *)p;
  p += irep->ilen * 4;
  int nn=0;
  for(nn = 0;nn<irep->ilen;nn++){
    show_opcode( bin_to_uint32(irep->code + (nn*4)) );
  }

  // POOL BLOCK
  irep->plen = bin_to_uint32(p);	p += 4;
  printf("irep->plen=%d\n",irep->plen);
  if( irep->plen ) {
    irep->pools = (mrb_object**)malloc(sizeof(void*) * irep->plen);
    if(irep->pools == NULL ) {
      vm->error_code = LOAD_FILE_IREP_ERROR_ALLOCATION;
      return NULL;
    }
  }

  int i;
  const uint8_t *pool_head_p = p;
  for( i = 0; i < irep->plen; i++ ) {
    int tt = *p++;
    int obj_size = bin_to_uint16(p);	p += 2;
    assert(obj_size <= MAX_LITERAL_LEN);
    
    mrb_object *obj = malloc(sizeof(mrb_object));
    if( obj == NULL ) {
      vm->error_code = LOAD_FILE_IREP_ERROR_ALLOCATION;
      return NULL;
    }
    switch( tt ) {
    case 0: { // IREP_TT_STRING
      obj->tt = MRB_TT_STRING;
      //obj->str = (char*)p;
    } break;
    case 1: { // IREP_TT_FIXNUM
      char buf[obj_size+1];
      memcpy(buf, p, obj_size);
      buf[obj_size] = '\0';
      obj->tt = MRB_TT_FIXNUM;
      //obj->i = atol(buf);
    } break;
    default:
      break;
    }

    irep->pools[i] = obj;
    p += obj_size;
  }
  irep->pool_head = pool_head_p;
  irep->pool_total_len = (uint16_t)(p - pool_head_p);

  // SYMS BLOCK
  irep->ptr_to_sym = (uint8_t*)p;
  int slen = bin_to_uint32(p);		p += 4;
  irep->slen = slen;
  printf("slen=%d\n",slen);
  while( --slen >= 0 ) {
    int s = bin_to_uint16(p);		p += 2;
    assert(s <= MAX_SYMBOL_LEN);
    p += s+1;
  }

  *pos = p;
  return irep;
}

static mrb_irep * load_irep_0(mrb_vm *vm, const uint8_t **pos)
{
  mrb_irep *irep = load_irep_1(vm, pos);
  if( !irep ) return NULL;

  int i;
  for( i = 0; i < irep->rlen; i++ ) {
    irep->reps[i] = load_irep_0(vm, pos);
  }

  return irep;
}


static int load_irep(mrb_vm *vm, const uint8_t **pos)
{
  const uint8_t *p = *pos + 4;			// 4 = skip "RITE"
  int section_size = bin_to_uint32(p);
  p += 4;
  if( memcmp(p, "0000", 4) != 0 ) {		// rite version
    vm->error_code = LOAD_FILE_IREP_ERROR_VERSION;
    return -1;
  }
  p += 4;
  vm->irep = load_irep_0(vm, &p);
  if( vm->irep == NULL ) {
    return -1;
  }

  *pos += section_size;
  return 0;
}


static int load_lvar(mrb_vm *vm, const uint8_t **pos)
{
  const uint8_t *p = *pos;

  /* size */
  *pos += bin_to_uint32(p+4);

  return 0;
}

int mrbc_load_mrb(mrb_vm *vm,const uint8_t *ptr)
{
  int ret = -1;
  const uint8_t* head = ptr;

  printf("** load header [%d]**\n",(int)(ptr-head));
  ret = load_header(vm, &ptr);
  while( ret == 0 ) {
    if( memcmp(ptr, "IREP", 4) == 0 ) {
      printf("** load IREP [%d]**\n",(int)(ptr-head));
      ret = load_irep(vm, &ptr);
    }
    else if( memcmp(ptr, "LVAR", 4) == 0 ) {
      printf("** load LVAR [%d]**\n",(int)(ptr-head));
      ret = load_lvar(vm, &ptr);
    }
    else if( memcmp(ptr, "END\0", 4) == 0 ) {
      printf("** END [%d]**\n",(int)(ptr-head));
      break;
    }
  }

  return ret;
}

uint8_t * load_mrb_file(const char *filename)
{
  FILE *fp = fopen(filename, "rb");

  if( fp == NULL ) {
    fprintf(stderr, "File not found\n");
    return NULL;
  }

  // get filesize
  fseek(fp, 0, SEEK_END);
  size_t size = ftell(fp);
  fseek(fp, 0, SEEK_SET);

  // allocate memory
  printf("file size = %d\n",(int)size);
  uint8_t *p = malloc(size);
  if( p != NULL ) {
    fread(p, sizeof(uint8_t), size, fp);
  } else {
    fprintf(stderr, "Memory allocate error.\n");
  }
  fclose(fp);

  return p;
}

int main(int argc, char *argv[]){
  if( argc != 2 ) {
    printf("Usage: %s <xxxx.mrb>\n", argv[0]);
    return 1;
  }

  uint8_t *mrbbuf = load_mrb_file( argv[1] );
  if( mrbbuf == 0 ) return 1;

  mrb_vm vm;
  memset(&vm,0,sizeof(mrb_vm));
  mrbc_load_mrb(&vm, mrbbuf);
  trans_code_mrb(&vm);
  
  return 0;
}

