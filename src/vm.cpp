#include <stdio.h>

#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <assert.h>
#include "micro_vm.h"
#include "class.h"
#include "symbol.h"
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
inline static int op_nop( mrb_mvm *vm, uint32_t code, mrb_value *regs )
{
  DEBUG_FPRINTLN("[OP_NOP]");
	return 0;
}

inline static int op_loadself( mrb_mvm *vm, uint32_t code, mrb_value *regs )
{
  DEBUG_FPRINTLN("[OP_LOADSELF]");
  int ra = GETARG_A(code);
  
  mrbc_release(&regs[ra]);
  mrbc_dup(&regs[0]);       // TODO: Need?
  regs[ra] = regs[0];
  return 0;
}

inline static int op_send( mrb_mvm *vm, uint32_t code, mrb_value *regs )
{
  DEBUG_FPRINTLN("[OP_SEND]");
#if 1
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

  mrb_sym sym_id = get_irep_symbol_id(vm->pc_irep,rb);
  mrb_proc *m = find_method(vm, recv, sym_id);
  
  if( m == 0 ) {
    cprintf("MethodNotFound %d\n", sym_id);
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
#endif
  return 0;
}

inline static int op_string( mrb_mvm *vm, uint32_t code, mrb_value *regs )
{
  DEBUG_FPRINTLN("[OP_STRING]");
  int ra = GETARG_A(code);
  int rb = GETARG_Bx(code);

  //mrb_object *pool_obj = vm->pc_irep->pools[rb]; 
  uint8_t str[MAX_LITERAL_LEN];
  uint16_t obj_size=0;
  get_irep_pool(str,&obj_size,vm->pc_irep,rb);
  cprintf("Len=%u Str=%s\n",obj_size,str);
  
  mrb_value value = mrbc_string_new(vm, str, obj_size);
  if( value.string == NULL ) return -1;
  
  mrbc_release(&regs[ra]);
  regs[ra] = value;
  return 0;
}

inline static int op_stop( mrb_mvm *vm, uint32_t code, mrb_value *regs )
{
  DEBUG_FPRINTLN("[OP_STOP]");
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
  DEBUG_FPRINTLN("start run_vm");
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

    switch( opcode ) {
    case OP_NOP:        ret = op_nop       (vm, code, regs); break;
//		  case OP_MOVE:       ret = op_move      (vm, code, regs); break;
//		  case OP_LOADL:      ret = op_loadl     (vm, code, regs); break;
//		  case OP_LOADI:      ret = op_loadi     (vm, code, regs); break;
//		  case OP_LOADSYM:    ret = op_loadsym   (vm, code, regs); break;
//		  case OP_LOADNIL:    ret = op_loadnil   (vm, code, regs); break;
		  case OP_LOADSELF:   ret = op_loadself  (vm, code, regs); break;
//		  case OP_LOADT:      ret = op_loadt     (vm, code, regs); break;
//		  case OP_LOADF:      ret = op_loadf     (vm, code, regs); break;
//		  case OP_GETGLOBAL:  ret = op_getglobal (vm, code, regs); break;
//		  case OP_SETGLOBAL:  ret = op_setglobal (vm, code, regs); break;
//		  case OP_GETIV:      ret = op_getiv     (vm, code, regs); break;
//		  case OP_SETIV:      ret = op_setiv     (vm, code, regs); break;
//		  case OP_GETCONST:   ret = op_getconst  (vm, code, regs); break;
//		  case OP_SETCONST:   ret = op_setconst  (vm, code, regs); break;
//		  case OP_GETUPVAR:   ret = op_getupvar  (vm, code, regs); break;
//		  case OP_SETUPVAR:   ret = op_setupvar  (vm, code, regs); break;
//		  case OP_JMP:        ret = op_jmp       (vm, code, regs); break;
//		  case OP_JMPIF:      ret = op_jmpif     (vm, code, regs); break;
//		  case OP_JMPNOT:     ret = op_jmpnot    (vm, code, regs); break;
    case OP_SEND:       ret = op_send      (vm, code, regs); break;
//		  case OP_SENDB:      ret = op_send      (vm, code, regs); break;  // reuse
//		  case OP_CALL:       ret = op_call      (vm, code, regs); break;
//		  case OP_ENTER:      ret = op_enter     (vm, code, regs); break;
//		  case OP_RETURN:     ret = op_return    (vm, code, regs); break;
//		  case OP_BLKPUSH:    ret = op_blkpush   (vm, code, regs); break;
//		  case OP_ADD:        ret = op_add       (vm, code, regs); break;
//		  case OP_ADDI:       ret = op_addi      (vm, code, regs); break;
//		  case OP_SUB:        ret = op_sub       (vm, code, regs); break;
//		  case OP_SUBI:       ret = op_subi      (vm, code, regs); break;
//		  case OP_MUL:        ret = op_mul       (vm, code, regs); break;
//		  case OP_DIV:        ret = op_div       (vm, code, regs); break;
//		  case OP_EQ:         ret = op_eq        (vm, code, regs); break;
//		  case OP_LT:         ret = op_lt        (vm, code, regs); break;
//		  case OP_LE:         ret = op_le        (vm, code, regs); break;
//		  case OP_GT:         ret = op_gt        (vm, code, regs); break;
//		  case OP_GE:         ret = op_ge        (vm, code, regs); break;
//		  case OP_ARRAY:      ret = op_array     (vm, code, regs); break;
		  case OP_STRING:     ret = op_string    (vm, code, regs); break;
//		  case OP_STRCAT:     ret = op_strcat    (vm, code, regs); break;
//		  case OP_HASH:       ret = op_hash      (vm, code, regs); break;
//		  case OP_LAMBDA:     ret = op_lambda    (vm, code, regs); break;
//		  case OP_RANGE:      ret = op_range     (vm, code, regs); break;
//		  case OP_CLASS:      ret = op_class     (vm, code, regs); break;
//		  case OP_EXEC:       ret = op_exec      (vm, code, regs); break;
//		  case OP_METHOD:     ret = op_method    (vm, code, regs); break;
//		  case OP_TCLASS:     ret = op_tclass    (vm, code, regs); break;
    case OP_STOP:       ret = op_stop      (vm, code, regs); break;
    case OP_ABORT:      ret = op_stop      (vm, code, regs); break;  // reuse
    default:
      console_printf("UNKNOWN OPCODE >> %02X\n",opcode);
      break;
    }
    hal_delay(1000);
  } while( !vm->flag_preemption );
  DEBUG_FPRINT("VM END\n");
  
  vm->flag_preemption = 0;
  
  return;
}

