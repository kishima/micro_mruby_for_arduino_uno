#include <stdio.h>

#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <assert.h>
#include "micro_vm.h"
#include "class.h"
#include "symbol.h"
#include "global.h"
#include "opcode.h"
#include "avr_access.h"
#include "console.h"
#include "debug.h"

#include "c_string.h"

#include <Arduino.h>
#include <avr/pgmspace.h>

//Only one VM is available
static mrb_mvm vm_body;

void init_vm(void){
  mrb_mvm* vm = &vm_body;
  //initialize VM
  memset(vm,0,sizeof(mrb_mvm));

  //set self
  vm->pc_irep = vm->irep;
  vm->pc = 0;
  vm->current_regs = vm->regs;

  // set self to reg[0]
  vm->regs[0].tt = MRB_TT_CLASS;
  vm->regs[0].cls = mrbc_class_object;
 
  memset(vm->callinfo, 0, sizeof(vm->callinfo));

  // target_class
  vm->target_class = mrbc_class_object;

}

mrb_mvm* get_vm(void){
  return &vm_body;
}
//================================================================
/*!@brief
  Push current status to callinfo stack
*/
void mrbc_push_callinfo(mrb_mvm *vm, int n_args)
{
  mrb_callinfo *callinfo = vm->callinfo + vm->callinfo_top;
  callinfo->current_regs = vm->current_regs;
  callinfo->pc_irep = vm->pc_irep;
  callinfo->pc = vm->pc;
  callinfo->n_args = n_args;
  callinfo->target_class = vm->target_class;
  vm->callinfo_top++;
}



//================================================================
/*!@brief
  Push current status to callinfo stack
*/
void mrbc_pop_callinfo(mrb_mvm *vm)
{
  vm->callinfo_top--;
  mrb_callinfo *callinfo = vm->callinfo + vm->callinfo_top;
  vm->current_regs = callinfo->current_regs;
  vm->pc_irep = callinfo->pc_irep;
  vm->pc = callinfo->pc;
  vm->target_class = callinfo->target_class;
}

//--------------------------------
//  OPCODE
//--------------------------------
//#define SHOW_OPCODE
#ifdef SHOW_OPCODE
inline void show_register(mrb_value v){
  switch(v.tt){
    case MRB_TT_HANDLE: DEBUG_PRINT("HANDLE"); break;
    case MRB_TT_EMPTY: DEBUG_PRINT("EMPTY"); break;
    case MRB_TT_NIL: DEBUG_PRINT("NIL"); break;
    case MRB_TT_FALSE: DEBUG_PRINT("FALSE"); break;
    case MRB_TT_TRUE: DEBUG_PRINT("TRUE"); break;
    case MRB_TT_FIXNUM:
      DEBUG_PRINT("FIXNUM"); 
      cprintf(":%d",v.i);
      break;
    case MRB_TT_FLOAT: DEBUG_PRINT("FLOAT"); break;
    case MRB_TT_SYMBOL: 
      DEBUG_PRINT("SYMBOL"); 
      cprintf(":%d",v.i);
      break;
    case MRB_TT_CLASS: 
      DEBUG_PRINT("CLASS"); 
      cprintf(":%d",v.cls->sym_id);
      break;
    case MRB_TT_OBJECT: DEBUG_PRINT("OBJECT"); break;
    case MRB_TT_PROC: DEBUG_PRINT("PROC"); break;
    case MRB_TT_ARRAY: DEBUG_PRINT("ARRAY"); break;
    case MRB_TT_STRING:
      DEBUG_PRINT("STRING"); 
      cprintf(":%s",v.str);
      break;
    case MRB_TT_RANGE: DEBUG_PRINT("RANGE"); break;
    case MRB_TT_HASH: DEBUG_PRINT("HASH"); break;
    default:DEBUG_PRINT("?"); break;
  }
  DEBUG_PRINT("\n");
}
void show_registers(mrb_value* v){
  int i ;
  for(i=0;i<8;i++){
    cprintf("  REG[%d]:",i);
    show_register(v[i]);
  }
}
void show_opcode_name(mrb_mvm *vm,uint32_t opcode){
  switch( opcode ) {
  case OP_NOP:        DEBUG_FPRINTLN("[OP_NOP]"); break;
  case OP_MOVE:       DEBUG_FPRINTLN("[OP_MOVE]"); break;
  case OP_LOADL:      DEBUG_FPRINTLN("[OP_LOADL]"); break;
  case OP_LOADI:      DEBUG_FPRINTLN("[OP_LOADI]"); break;
  case OP_LOADSYM:    DEBUG_FPRINTLN("[OP_LOADSYM]"); break;
  case OP_LOADNIL:    DEBUG_FPRINTLN("[OP_LOADNIL]"); break;
  case OP_LOADSELF:   DEBUG_FPRINTLN("[OP_LOADSELF]"); break;
  case OP_LOADT:      DEBUG_FPRINTLN("[OP_LOADT]"); break;
  case OP_LOADF:      DEBUG_FPRINTLN("[OP_LOADF]"); break;
  case OP_GETGLOBAL:  DEBUG_FPRINTLN("[OP_GETGLOBAL]"); break;
  case OP_SETGLOBAL:  DEBUG_FPRINTLN("[OP_SETGLOBAL]"); break;
  case OP_GETIV:      DEBUG_FPRINTLN("[OP_GETIV]"); break;
  case OP_SETIV:      DEBUG_FPRINTLN("[OP_SETIV]"); break;
  case OP_GETCONST:   DEBUG_FPRINTLN("[OP_GETCONST]"); break;
  case OP_SETCONST:   DEBUG_FPRINTLN("[OP_SETCONST]"); break;
  case OP_GETUPVAR:   DEBUG_FPRINTLN("[OP_GETUPVAR]"); break;
  case OP_SETUPVAR:   DEBUG_FPRINTLN("[OP_SETUPVAR]"); break;
  case OP_JMP:        DEBUG_FPRINTLN("[OP_JMP]"); break;
  case OP_JMPIF:      DEBUG_FPRINTLN("[OP_JMPIF]"); break;
  case OP_JMPNOT:     DEBUG_FPRINTLN("[OP_JMPNOT]"); break;
  case OP_SEND:       DEBUG_FPRINTLN("[OP_SEND]"); break;
  case OP_SENDB:      DEBUG_FPRINTLN("[OP_SENDB]"); break;  // reuse
  case OP_CALL:       DEBUG_FPRINTLN("[OP_CALL]"); break;
  case OP_ENTER:      DEBUG_FPRINTLN("[OP_ENTER]"); break;
  case OP_RETURN:     DEBUG_FPRINTLN("[OP_RETURN]"); break;
  case OP_BLKPUSH:    DEBUG_FPRINTLN("[OP_BLKPUSH]"); break;
  case OP_ADD:        DEBUG_FPRINTLN("[OP_ADD]"); break;
  case OP_ADDI:       DEBUG_FPRINTLN("[OP_ADDI]"); break;
  case OP_SUB:        DEBUG_FPRINTLN("[OP_SUB]"); break;
  case OP_SUBI:       DEBUG_FPRINTLN("[OP_SUBI]"); break;
  case OP_MUL:        DEBUG_FPRINTLN("[OP_MUL]"); break;
  case OP_DIV:        DEBUG_FPRINTLN("[OP_DIV]"); break;
  case OP_EQ:         DEBUG_FPRINTLN("[OP_EQ]"); break;
  case OP_LT:         DEBUG_FPRINTLN("[OP_LT]"); break;
  case OP_LE:         DEBUG_FPRINTLN("[OP_LE]"); break;
  case OP_GT:         DEBUG_FPRINTLN("[OP_GT]"); break;
  case OP_GE:         DEBUG_FPRINTLN("[OP_GE]"); break;
  case OP_ARRAY:      DEBUG_FPRINTLN("[OP_ARRAY]"); break;
  case OP_STRING:     DEBUG_FPRINTLN("[OP_STRING]"); break;
  case OP_STRCAT:     DEBUG_FPRINTLN("[OP_STRCAT]"); break;
  case OP_HASH:       DEBUG_FPRINTLN("[OP_HASH]"); break;
  case OP_LAMBDA:     DEBUG_FPRINTLN("[OP_LAMBDA");break;
  case OP_RANGE:      DEBUG_FPRINTLN("[OP_RANGE]"); break;
  case OP_CLASS:      DEBUG_FPRINTLN("[OP_CLASS]"); break;
  case OP_EXEC:       DEBUG_FPRINTLN("[OP_EXEC]"); break;
  case OP_METHOD:     DEBUG_FPRINTLN("[OP_METHOD]"); break;
  case OP_TCLASS:     DEBUG_FPRINTLN("[OP_TCLASS]"); break;
  case OP_STOP:       DEBUG_FPRINTLN("[OP_STOP]"); break;
  case OP_ABORT:      DEBUG_FPRINTLN("[OP_ABORT]"); break;  // reuse
  default:
    DEBUG_FPRINTLN("[UNKNOWN]");
    break;
  }
  show_registers(vm->current_regs);
}
#endif

inline void not_supported(){
  //
}

inline static int op_nop( mrb_mvm *vm, uint32_t code, mrb_value *regs )
{
  return 0;
}

inline static int op_move( mrb_mvm *vm, uint32_t code, mrb_value *regs )
{
  int ra = GETARG_A(code);
  int rb = GETARG_B(code);

  mrbc_release(&regs[ra]);
  mrbc_dup(&regs[rb]);
  regs[ra] = regs[rb];

  return 0;
}

inline static int op_loadl( mrb_mvm *vm, uint32_t code, mrb_value *regs )
{
  int ra = GETARG_A(code);
  int rb = GETARG_Bx(code);

  mrbc_release(&regs[ra]);

  // regs[ra] = vm->pc_irep->pools[rb];

  //mrb_object *pool_obj = vm->pc_irep->pools[rb];
  mrb_object pool_obj;
  get_irep_pool((uint8_t*)&regs[ra],NULL,vm->pc_irep,rb);
  regs[ra] = pool_obj;

  return 0;
}

inline static int op_loadi( mrb_mvm *vm, uint32_t code, mrb_value *regs )
{
  int ra = GETARG_A(code);

  mrbc_release(&regs[ra]);
  regs[ra].tt = MRB_TT_FIXNUM;
  regs[ra].i = GETARG_sBx(code);

  return 0;
}

inline static int op_loadsym( mrb_mvm *vm, uint32_t code, mrb_value *regs )
{
  int ra = GETARG_A(code);
  int rb = GETARG_Bx(code);
  mrb_sym sym_id = get_irep_symbol_id(vm->pc_irep,rb);

  mrbc_release(&regs[ra]);
  regs[ra].tt = MRB_TT_SYMBOL;
  regs[ra].i = sym_id;

  return 0;
}

inline static int op_loadnil( mrb_mvm *vm, uint32_t code, mrb_value *regs )
{
  int ra = GETARG_A(code);

  mrbc_release(&regs[ra]);
  regs[ra].tt = MRB_TT_NIL;

  return 0;
}

inline static int op_loadself( mrb_mvm *vm, uint32_t code, mrb_value *regs )
{
  int ra = GETARG_A(code);

  mrbc_release(&regs[ra]);
  mrbc_dup(&regs[0]);       // TODO: Need?
  regs[ra] = regs[0];
  return 0;
}

inline static int op_loadt( mrb_mvm *vm, uint32_t code, mrb_value *regs )
{
  int ra = GETARG_A(code);

  mrbc_release(&regs[ra]);
  regs[ra].tt = MRB_TT_TRUE;

  return 0;
}

inline static int op_loadf( mrb_mvm *vm, uint32_t code, mrb_value *regs )
{
  int ra = GETARG_A(code);

  mrbc_release(&regs[ra]);
  regs[ra].tt = MRB_TT_FALSE;

  return 0;
}

inline static int op_getglobal( mrb_mvm *vm, uint32_t code, mrb_value *regs )
{
  int ra = GETARG_A(code);
  int rb = GETARG_Bx(code);
  //const char *sym_name = mrbc_get_irep_symbol(vm->pc_irep->ptr_to_sym, rb);
  //mrb_sym sym_id = str_to_symid(sym_name);
  mrb_sym sym_id = get_irep_symbol_id(vm->pc_irep,rb);

  mrbc_release(&regs[ra]);
  regs[ra] = global_object_get(sym_id);

  return 0;
}


inline static int op_setglobal( mrb_mvm *vm, uint32_t code, mrb_value *regs )
{
  int ra = GETARG_A(code);
  int rb = GETARG_Bx(code);
  //const char *sym_name = mrbc_get_irep_symbol(vm->pc_irep->ptr_to_sym, rb);
  //mrb_sym sym_id = str_to_symid(sym_name);
  mrb_sym sym_id = get_irep_symbol_id(vm->pc_irep,rb);

  global_object_add(sym_id, regs[ra]);

  return 0;
}

inline static int op_getiv( mrb_mvm *vm, uint32_t code, mrb_value *regs )
{
  int ra = GETARG_A(code);
  int rb = GETARG_Bx(code);

  //const char *sym_name = mrbc_get_irep_symbol(vm->pc_irep->ptr_to_sym, rb);
  //mrb_sym sym_id = str_to_symid(sym_name+1);	// skip '@'
  mrb_sym sym_id = get_irep_symbol_id(vm->pc_irep,rb);

  //TODO
  //mrb_value val = mrbc_instance_getiv(&regs[0], sym_id);
  mrb_value val;

  mrbc_release(&regs[ra]);
  regs[ra] = val;

  return 0;
}

inline static int op_setiv( mrb_mvm *vm, uint32_t code, mrb_value *regs )
{
  int ra = GETARG_A(code);
  int rb = GETARG_Bx(code);

  //const char *sym_name = mrbc_get_irep_symbol(vm->pc_irep->ptr_to_sym, rb);
  //mrb_sym sym_id = str_to_symid(sym_name+1);	// skip '@'
  mrb_sym sym_id = get_irep_symbol_id(vm->pc_irep,rb);

  //TODO
  //mrbc_instance_setiv(&regs[0], sym_id, &regs[ra]);

  return 0;
}

inline static int op_getconst( mrb_mvm *vm, uint32_t code, mrb_value *regs )
{
  int ra = GETARG_A(code);
  int rb = GETARG_Bx(code);
  //const char *sym_name = mrbc_get_irep_symbol(vm->pc_irep->ptr_to_sym, rb);
  //mrb_sym sym_id = str_to_symid(sym_name);
  mrb_sym sym_id = get_irep_symbol_id(vm->pc_irep,rb);

  mrbc_release(&regs[ra]);
  regs[ra] = const_object_get(sym_id);

  return 0;
}

inline static int op_setconst( mrb_mvm *vm, uint32_t code, mrb_value *regs ) {
  int ra = GETARG_A(code);
  int rb = GETARG_Bx(code);
  //const char *sym_name = mrbc_get_irep_symbol(vm->pc_irep->ptr_to_sym, rb);
  //mrb_sym sym_id = str_to_symid(sym_name);
  mrb_sym sym_id = get_irep_symbol_id(vm->pc_irep,rb);

  const_object_add(sym_id, &regs[ra]);

  return 0;
}

inline static int op_getupvar( mrb_mvm *vm, uint32_t code, mrb_value *regs )
{
  int ra = GETARG_A(code);
  int rb = GETARG_B(code);
  int rc = GETARG_C(code);   // UP
  
  mrb_callinfo *callinfo = vm->callinfo + vm->callinfo_top - 2 - rc;
  mrb_value *up_regs = callinfo->current_regs;
  
  mrbc_release( &regs[ra] );
  mrbc_dup( &up_regs[rb] );
  regs[ra] = up_regs[rb];
  
  return 0;
}


inline static int op_setupvar( mrb_mvm *vm, uint32_t code, mrb_value *regs )
{
  int ra = GETARG_A(code);
  int rb = GETARG_B(code);
  int rc = GETARG_C(code);   // UP
  
  mrb_callinfo *callinfo = vm->callinfo + vm->callinfo_top - 2 - rc;
  mrb_value *up_regs = callinfo->current_regs;

  mrbc_release( &up_regs[rb] );
  mrbc_dup( &regs[ra] );
  up_regs[rb] = regs[ra];

  return 0;
}

inline static int op_jmp( mrb_mvm *vm, uint32_t code, mrb_value *regs )
{
  vm->pc += GETARG_sBx(code) - 1;
  return 0;
}


inline static int op_jmpif( mrb_mvm *vm, uint32_t code, mrb_value *regs )
{
  if( regs[GETARG_A(code)].tt > MRB_TT_FALSE ) {
    vm->pc += GETARG_sBx(code) - 1;
  }
  return 0;
}

inline static int op_jmpnot( mrb_mvm *vm, uint32_t code, mrb_value *regs )
{
  if( regs[GETARG_A(code)].tt <= MRB_TT_FALSE ) {
    vm->pc += GETARG_sBx(code) - 1;
  }
  return 0;
}


inline static int op_send( mrb_mvm *vm, uint32_t code, mrb_value *regs )
{
  int ra = GETARG_A(code);
  int rb = GETARG_B(code);  // index of method sym
  int rc = GETARG_C(code);  // number of params
  mrb_value recv = regs[ra];
  
  // Block param
  int bidx = ra + rc + 1;
  switch( GET_OPCODE(code) ) {
  case OP_SEND:
    // set nil
    mrbc_release( &regs[bidx] );
    regs[bidx].tt = MRB_TT_NIL;
    break;
    
  case OP_SENDB:
    if( regs[bidx].tt != MRB_TT_NIL && regs[bidx].tt != MRB_TT_PROC ){
      return 0;
    }
    break;
    
  default:
    break;
  }

  //DEBUG_FPRINTLN("find medhod");delay(500);
  mrb_sym sym_id = get_irep_symbol_id(vm->pc_irep,rb);
  //cprintf("find sym:%d ra=%d\n", sym_id, ra);
  mrb_proc *m = find_method(recv, sym_id);
  
  if( m == 0 ) {
    cprintf("MethodNotFound %d\n", sym_id);
    return 0;
  }
  
  if(IS_PGM(m)){
    mrb_func_t func = find_c_funcs(m);
    //cprintf("pgm %d %p\n",m,func);delay(500);
    func(vm, regs + ra, rc);
    int release_reg = ra+rc+1;
    while( release_reg <= bidx ) {
      // mrbc_release(&regs[release_reg]);
      release_reg++;
    }
    return 0;
  }

  // m is C func
  if( m->c_func ) {
    m->func(vm, regs + ra, rc);
    
    int release_reg = ra+rc+1;
    while( release_reg <= bidx ) {
      // mrbc_release(&regs[release_reg]);
      release_reg++;
    }
    return 0;
  }
  
  // m is Ruby method.
  // callinfo
  mrbc_push_callinfo(vm, rc);
  
  // target irep
  vm->pc = 0;
  vm->pc_irep = m->irep;
  
  // new regs
  vm->current_regs += ra;
  return 0;
}

inline static int op_enter( mrb_mvm *vm, uint32_t code, mrb_value *regs )
{
  mrb_callinfo *callinfo = vm->callinfo + vm->callinfo_top - 1;
  uint32_t enter_param = GETARG_Ax(code);
  int def_args = (enter_param >> 13) & 0x1f;  // default args
  int args = (enter_param >> 18) & 0x1f;      // given args
  if( def_args > 0 ){
    vm->pc += callinfo->n_args - args;
  }
  return 0;
}

inline static int op_call( mrb_mvm *vm, uint32_t code, mrb_value *regs )
{
  mrbc_push_callinfo(vm, 0);

  // jump to proc
  vm->pc = 0;
  vm->pc_irep = regs[0].proc->irep;

  return 0;
}

inline static int op_return( mrb_mvm *vm, uint32_t code, mrb_value *regs )
{
  // return value
  int ra = GETARG_A(code);
  mrb_value v = regs[ra];
  mrbc_dup(&v);
  mrbc_release(&regs[0]);
  regs[0] = v;
  //  }
  // restore irep,pc,regs
  vm->callinfo_top--;
  mrb_callinfo *callinfo = vm->callinfo + vm->callinfo_top;
  mrb_value *regs_ptr = vm->current_regs;
  vm->current_regs = callinfo->current_regs;

  vm->pc_irep = callinfo->pc_irep;
  vm->pc = callinfo->pc;
  vm->target_class = callinfo->target_class;
  return 0;
}

inline static int op_blkpush( mrb_mvm *vm, uint32_t code, mrb_value *regs )
{
  int ra = GETARG_A(code);

  mrb_value *stack = regs + 1;

  if( stack[0].tt == MRB_TT_NIL ){
    return -1;  // EYIELD
  }

  mrbc_release(&regs[ra]);
  mrbc_dup( stack );
  regs[ra] = stack[0];

  return 0;
}

inline static int op_add( mrb_mvm *vm, uint32_t code, mrb_value *regs )
{
  int ra = GETARG_A(code);

  if( regs[ra].tt == MRB_TT_FIXNUM ) {
    if( regs[ra+1].tt == MRB_TT_FIXNUM ) {	// in case of Fixnum, Fixnum
      regs[ra].i += regs[ra+1].i;
      return 0;
    }

  }

  // other case
  op_send(vm, code, regs);
  return 0;
}

inline static int op_addi( mrb_mvm *vm, uint32_t code, mrb_value *regs )
{
  int ra = GETARG_A(code);

  if( regs[ra].tt == MRB_TT_FIXNUM ) {
    regs[ra].i += GETARG_C(code);
    return 0;
  }

  not_supported();
  return 0;
}

inline static int op_sub( mrb_mvm *vm, uint32_t code, mrb_value *regs )
{
  int ra = GETARG_A(code);

  if( regs[ra].tt == MRB_TT_FIXNUM ) {
    if( regs[ra+1].tt == MRB_TT_FIXNUM ) {	// in case of Fixnum, Fixnum
      regs[ra].i -= regs[ra+1].i;
      return 0;
    }
  }

  // other case
  op_send(vm, code, regs);
  mrbc_release(&regs[ra+1]);
  return 0;
}

inline static int op_subi( mrb_mvm *vm, uint32_t code, mrb_value *regs )
{
  int ra = GETARG_A(code);

  if( regs[ra].tt == MRB_TT_FIXNUM ) {
    regs[ra].i -= GETARG_C(code);
    return 0;
  }

  not_supported();
  return 0;
}

inline static int op_mul( mrb_mvm *vm, uint32_t code, mrb_value *regs )
{
  int ra = GETARG_A(code);

  if( regs[ra].tt == MRB_TT_FIXNUM ) {
    if( regs[ra+1].tt == MRB_TT_FIXNUM ) {	// in case of Fixnum, Fixnum
      regs[ra].i *= regs[ra+1].i;
      return 0;
    }
  }

  // other case
  op_send(vm, code, regs);
  mrbc_release(&regs[ra+1]);
  return 0;
}

inline static int op_div( mrb_mvm *vm, uint32_t code, mrb_value *regs )
{
  int ra = GETARG_A(code);

  if( regs[ra].tt == MRB_TT_FIXNUM ) {
    if( regs[ra+1].tt == MRB_TT_FIXNUM ) {	// in case of Fixnum, Fixnum
      regs[ra].i /= regs[ra+1].i;
      return 0;
    }
  }

  // other case
  op_send(vm, code, regs);
  mrbc_release(&regs[ra+1]);
  return 0;
}

inline static int op_eq( mrb_mvm *vm, uint32_t code, mrb_value *regs )
{
  int ra = GETARG_A(code);
  int result = mrbc_compare(&regs[ra], &regs[ra+1]);

  mrbc_release(&regs[ra+1]);
  mrbc_release(&regs[ra]);
  regs[ra].tt = result ? MRB_TT_FALSE : MRB_TT_TRUE;

  return 0;
}

inline static int op_lt( mrb_mvm *vm, uint32_t code, mrb_value *regs )
{
  int ra = GETARG_A(code);
  int result;

  if( regs[ra].tt == MRB_TT_FIXNUM ) {
    if( regs[ra+1].tt == MRB_TT_FIXNUM ) {
      result = regs[ra].i < regs[ra+1].i;	// in case of Fixnum, Fixnum
      goto DONE;
    }
  }

  // other case
  op_send(vm, code, regs);
  mrbc_release(&regs[ra+1]);
  return 0;

DONE:
  regs[ra].tt = result ? MRB_TT_TRUE : MRB_TT_FALSE;
  return 0;
}


inline static int op_le( mrb_mvm *vm, uint32_t code, mrb_value *regs )
{
  int ra = GETARG_A(code);
  int result;

  if( regs[ra].tt == MRB_TT_FIXNUM ) {
    if( regs[ra+1].tt == MRB_TT_FIXNUM ) {
      result = regs[ra].i <= regs[ra+1].i;	// in case of Fixnum, Fixnum
      goto DONE;
    }
  }

  // other case
  op_send(vm, code, regs);
  mrbc_release(&regs[ra+1]);
  return 0;

DONE:
  regs[ra].tt = result ? MRB_TT_TRUE : MRB_TT_FALSE;
  return 0;
}


inline static int op_gt( mrb_mvm *vm, uint32_t code, mrb_value *regs )
{
  int ra = GETARG_A(code);
  int result;

  if( regs[ra].tt == MRB_TT_FIXNUM ) {
    if( regs[ra+1].tt == MRB_TT_FIXNUM ) {
      result = regs[ra].i > regs[ra+1].i;	// in case of Fixnum, Fixnum
      goto DONE;
    }
  }

  // other case
  op_send(vm, code, regs);
  mrbc_release(&regs[ra+1]);
  return 0;

DONE:
  regs[ra].tt = result ? MRB_TT_TRUE : MRB_TT_FALSE;
  return 0;
}

inline static int op_ge( mrb_mvm *vm, uint32_t code, mrb_value *regs )
{
  int ra = GETARG_A(code);
  int result;

  if( regs[ra].tt == MRB_TT_FIXNUM ) {
    if( regs[ra+1].tt == MRB_TT_FIXNUM ) {
      result = regs[ra].i >= regs[ra+1].i;	// in case of Fixnum, Fixnum
      goto DONE;
    }
  }

  // other case
  op_send(vm, code, regs);
  mrbc_release(&regs[ra+1]);
  return 0;

DONE:
  regs[ra].tt = result ? MRB_TT_TRUE : MRB_TT_FALSE;
  return 0;
}

inline static int op_array( mrb_mvm *vm, uint32_t code, mrb_value *regs )
{
  //TODO
  #if 0
  int ra = GETARG_A(code);
  int rb = GETARG_B(code);
  int rc = GETARG_C(code);

  mrb_value value; = mrbc_array_new(vm, rc);
  if( value.array == NULL ) return -1;	// ENOMEM

  memcpy( value.array->data, &regs[rb], sizeof(mrb_value) * rc );
  memset( &regs[rb], 0, sizeof(mrb_value) * rc );
  value.array->n_stored = rc;

  mrbc_release(&regs[ra]);
  regs[ra] = value;
#endif
  return 0;
}

inline static int op_string( mrb_mvm *vm, uint32_t code, mrb_value *regs )
{
  int ra = GETARG_A(code);
  int rb = GETARG_Bx(code);

  uint8_t str[MAX_LITERAL_LEN];
  uint16_t obj_size=0;
  get_irep_pool(str,&obj_size,vm->pc_irep,rb);
  //cprintf("Len=%u Str=%s\n",obj_size,str);
  
  mrb_value value = mrbc_string_new(str, obj_size);
  if( value.string == NULL ) return -1;
  
  mrbc_release(&regs[ra]);
  regs[ra] = value;
  return 0;
}

inline static int op_strcat( mrb_mvm *vm, uint32_t code, mrb_value *regs )
{
  int ra = GETARG_A(code);
  int rb = GETARG_B(code);

  // call "to_s"
  mrb_sym sym_id = str_to_symid("to_s");
  mrb_proc *m;
  m = find_method(regs[ra], sym_id);
  if( m && m->c_func ){
    m->func(vm, regs+ra, 0);
  } 
  m = find_method(regs[rb], sym_id);
  if( m && m->c_func ){
    m->func(vm, regs+rb, 0);
  }

  mrb_value v = mrbc_string_add(&regs[ra], &regs[rb]);
  mrbc_release(&regs[ra]);
  regs[ra] = v;
  return 0;
}

inline static int op_lambda( mrb_mvm *vm, uint32_t code, mrb_value *regs )
{
  int ra = GETARG_A(code);
  int rb = GETARG_Bz(code);      // sequence position in irep list
  // int c = GETARG_C(code);    // TODO: Add flags support for OP_LAMBDA
  mrb_proc *proc = mrbc_rproc_alloc("()");

  proc->c_func = 0;
  //proc->irep = vm->pc_irep->reps[rb];
  proc->irep = get_irep_irep_id(vm->pc_irep,rb);

  mrbc_release(&regs[ra]);
  regs[ra].tt = MRB_TT_PROC;
  regs[ra].proc = proc;

  return 0;
}

inline static int op_range( mrb_mvm *vm, uint32_t code, mrb_value *regs )
{
  //TODO
  #if 0
  int ra = GETARG_A(code);
  int rb = GETARG_B(code);
  int rc = GETARG_C(code);

  mrbc_dup(&regs[rb]);
  mrbc_dup(&regs[rb+1]);

  mrb_value value = mrbc_range_new(vm, &regs[rb], &regs[rb+1], rc);
  if( value.range == NULL ) return -1;		// ENOMEM

  mrbc_release(&regs[ra]);
  regs[ra] = value;
  #endif
  return 0;
}

inline static int op_class( mrb_mvm *vm, uint32_t code, mrb_value *regs )
{
  //TODO symid to str
  #if 0
  int ra = GETARG_A(code);
  int rb = GETARG_B(code);

  mrb_irep *cur_irep = vm->pc_irep;
  const char *sym_name = mrbc_get_irep_symbol(cur_irep->ptr_to_sym, rb);
  mrb_class *super = (regs[ra+1].tt == MRB_TT_CLASS) ? regs[ra+1].cls : mrbc_class_object;

  mrb_class *cls = mrbc_define_class(vm, sym_name, super);

  mrb_value ret = {.tt = MRB_TT_CLASS};
  ret.cls = cls;

  regs[ra] = ret;
#endif
  return 0;
}

inline static int op_exec( mrb_mvm *vm, uint32_t code, mrb_value *regs )
{
  int ra = GETARG_A(code);
  int rb = GETARG_Bx(code);

  mrb_value recv = regs[ra];

  // prepare callinfo
  mrb_callinfo *callinfo = vm->callinfo + vm->callinfo_top;
  callinfo->current_regs = vm->current_regs;
  callinfo->pc_irep = vm->pc_irep;
  callinfo->pc = vm->pc;
  callinfo->target_class = vm->target_class;
  callinfo->n_args = 0;
  vm->callinfo_top++;

  // target irep
  vm->pc = 0;
  vm->pc_irep = get_irep_irep_id(vm->irep,rb);

  // new regs
  vm->current_regs += ra;

  vm->target_class = find_class_by_object(&recv);

  return 0;
}

inline static int op_method( mrb_mvm *vm, uint32_t code, mrb_value *regs )
{
  int ra = GETARG_A(code);
  int rb = GETARG_B(code);
  mrb_proc *proc = regs[ra+1].proc;

  if( regs[ra].tt == MRB_TT_CLASS ) {
    mrb_class *cls = regs[ra].cls;

    // sym_id : method name
    mrb_irep_id cur_irep = vm->pc_irep;
    mrb_sym sym_id = get_irep_symbol_id(cur_irep,rb);

    // check same name method
    mrb_proc *p = cls->procs;
    void *pp = &cls->procs;
    while( p != NULL ) {
      if( p->sym_id == sym_id ) break;
      pp = &p->next;
      p = p->next;
    }
    if( p ) {
      // found it.
      *((mrb_proc**)pp) = p->next;
      if( !p->c_func ) {
        mrb_value v = {.tt = MRB_TT_PROC};
        v.proc = p;
        mrbc_release(&v);
      }
    }

    // add proc to class
    proc->c_func = 0;
    proc->sym_id = sym_id;
    proc->next = cls->procs;
    cls->procs = proc;

    regs[ra+1].tt = MRB_TT_EMPTY;
  }

  return 0;
}

inline static int op_tclass( mrb_mvm *vm, uint32_t code, mrb_value *regs )
{
  int ra = GETARG_A(code);

  mrbc_release(&regs[ra]);
  regs[ra].tt = MRB_TT_CLASS;
  regs[ra].cls = vm->target_class;

  return 0;
}


inline static int op_stop( mrb_mvm *vm, uint32_t code, mrb_value *regs )
{
  if( GET_OPCODE(code) == OP_STOP ) {
    int i;
    for( i = 0; i < MAX_REGS_SIZE; i++ ) {
      //mrbc_release(&vm->regs[i]);
    }
  }
  
  vm->flag_preemption = 1;
  return -1;
}

void run_vm(void){
  DEBUG_FPRINTLN("<VM START>");
  mrb_mvm *vm = &vm_body;
  int ret = 0;
  
  do {
    // get one bytecode
    uint32_t code = read_bytecode(vm->pc_irep,vm->pc);
    code = bin_to_uint32(&code);
    vm->pc++;
    
    // regs
    mrb_value *regs = vm->current_regs;
    
    // Dispatch
    uint32_t opcode = GET_OPCODE(code);
#ifdef SHOW_OPCODE
    show_opcode_name(vm,opcode);
#endif

    switch( opcode ) {
      case OP_NOP:        ret = op_nop       (vm, code, regs); break;
		  case OP_MOVE:       ret = op_move      (vm, code, regs); break;
		  case OP_LOADL:      ret = op_loadl     (vm, code, regs); break;
		  case OP_LOADI:      ret = op_loadi     (vm, code, regs); break;
		  case OP_LOADSYM:    ret = op_loadsym   (vm, code, regs); break;
		  case OP_LOADNIL:    ret = op_loadnil   (vm, code, regs); break;
		  case OP_LOADSELF:   ret = op_loadself  (vm, code, regs); break;
		  case OP_LOADT:      ret = op_loadt     (vm, code, regs); break;
		  case OP_LOADF:      ret = op_loadf     (vm, code, regs); break;
		  case OP_GETGLOBAL:  ret = op_getglobal (vm, code, regs); break;
		  case OP_SETGLOBAL:  ret = op_setglobal (vm, code, regs); break;
		  case OP_GETIV:      ret = op_getiv     (vm, code, regs); break;
		  case OP_SETIV:      ret = op_setiv     (vm, code, regs); break;
		  case OP_GETCONST:   ret = op_getconst  (vm, code, regs); break;
		  case OP_SETCONST:   ret = op_setconst  (vm, code, regs); break;
		  case OP_GETUPVAR:   ret = op_getupvar  (vm, code, regs); break;
		  case OP_SETUPVAR:   ret = op_setupvar  (vm, code, regs); break;
		  case OP_JMP:        ret = op_jmp       (vm, code, regs); break;
		  case OP_JMPIF:      ret = op_jmpif     (vm, code, regs); break;
		  case OP_JMPNOT:     ret = op_jmpnot    (vm, code, regs); break;
      case OP_SEND:       ret = op_send      (vm, code, regs); break;
		  case OP_SENDB:      ret = op_send      (vm, code, regs); break;  // reuse
		  case OP_CALL:       ret = op_call      (vm, code, regs); break;
		  case OP_ENTER:      ret = op_enter     (vm, code, regs); break;
		  case OP_RETURN:     ret = op_return    (vm, code, regs); break;
		  case OP_BLKPUSH:    ret = op_blkpush   (vm, code, regs); break;
		  case OP_ADD:        ret = op_add       (vm, code, regs); break;
		  case OP_ADDI:       ret = op_addi      (vm, code, regs); break;
		  case OP_SUB:        ret = op_sub       (vm, code, regs); break;
		  case OP_SUBI:       ret = op_subi      (vm, code, regs); break;
		  case OP_MUL:        ret = op_mul       (vm, code, regs); break;
		  case OP_DIV:        ret = op_div       (vm, code, regs); break;
		  case OP_EQ:         ret = op_eq        (vm, code, regs); break;
		  case OP_LT:         ret = op_lt        (vm, code, regs); break;
		  case OP_LE:         ret = op_le        (vm, code, regs); break;
		  case OP_GT:         ret = op_gt        (vm, code, regs); break;
		  case OP_GE:         ret = op_ge        (vm, code, regs); break;
		  case OP_ARRAY:      ret = op_array     (vm, code, regs); break;
		  case OP_STRING:     ret = op_string    (vm, code, regs); break;
		  case OP_STRCAT:     ret = op_strcat    (vm, code, regs); break;
//		  case OP_HASH:       ret = op_hash      (vm, code, regs); break;
		  case OP_LAMBDA:     ret = op_lambda    (vm, code, regs); break;
		  case OP_RANGE:      ret = op_range     (vm, code, regs); break;
		  case OP_CLASS:      ret = op_class     (vm, code, regs); break;
		  case OP_EXEC:       ret = op_exec      (vm, code, regs); break;
		  case OP_METHOD:     ret = op_method    (vm, code, regs); break;
		  case OP_TCLASS:     ret = op_tclass    (vm, code, regs); break;
      case OP_STOP:       ret = op_stop      (vm, code, regs); break;
      case OP_ABORT:      ret = op_stop      (vm, code, regs); break;  // reuse
      default:
        cprintf("UNKNOWN >> %02X\n",opcode);
      break;
    }
    #ifdef SHOW_OPCODE
    hal_delay(500);
    #endif
  } while( !vm->flag_preemption );
  DEBUG_FPRINT("<VM END>\n");
  
  vm->flag_preemption = 0;
  
  return;
}

