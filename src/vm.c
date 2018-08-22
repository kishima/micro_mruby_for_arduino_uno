#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <assert.h>
#include "vm.h"
#include "opcode.h"
#include "debug.h"

#include <avr/pgmspace.h>

static mrb_vm vm_body;

void init_vm(void){
  //initialize VM
}


inline static int op_nop( mrb_vm *vm, uint32_t code, mrb_value *regs )
{
	return 0;
}

inline static int op_loadself( mrb_vm *vm, uint32_t code, mrb_value *regs )
{
#if 0
	int ra = GETARG_A(code);
	
	mrbc_release(&regs[ra]);
	mrbc_dup(&regs[0]);       // TODO: Need?
	regs[ra] = regs[0];
#endif	
	return 0;
}

inline static int op_send( mrb_vm *vm, uint32_t code, mrb_value *regs )
{
#if 0
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
	
	const char *sym_name = mrbc_get_irep_symbol(vm->pc_irep->ptr_to_sym, rb);
	mrb_sym sym_id = str_to_symid(sym_name);
	mrb_proc *m = find_method(vm, recv, sym_id);
	
	if( m == 0 ) {
		console_printf("No method. vtype=%d method='%s'\n", recv.tt, sym_name);
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

inline static int op_string( mrb_vm *vm, uint32_t code, mrb_value *regs )
{
#if 0
	int ra = GETARG_A(code);
	int rb = GETARG_Bx(code);
	mrb_object *pool_obj = vm->pc_irep->pools[rb];
	
	/* CAUTION: pool_obj->str - 2. see IREP POOL structure. */
	int len = bin_to_uint16(pool_obj->str - 2);
	mrb_value value = mrbc_string_new(vm, pool_obj->str, len);
	if( value.string == NULL ) return -1;		// ENOMEM
	
	mrbc_release(&regs[ra]);
	regs[ra] = value;
#endif
	return 0;
}

inline static int op_stop( mrb_vm *vm, uint32_t code, mrb_value *regs )
{
#if 0
	if( GET_OPCODE(code) == OP_STOP ) {
		int i;
		for( i = 0; i < MAX_REGS_SIZE; i++ ) {
			mrbc_release(&vm->regs[i]);
		}
	}
	
	vm->flag_preemption = 1;
#endif	
	return -1;
}

void run_vm(void){
  DEBUG_PRINTLN("start run_vm");
  mrb_vm *vm = &vm_body;
  int ret = 0;
  
  do {
    // get one bytecode
    uint32_t code = bin_to_uint32(vm->pc_irep->code + vm->pc * 4);
    vm->pc++;
    
    // regs
    mrb_value *regs = vm->current_regs;
    
    // Dispatch
    int opcode = GET_OPCODE(code);
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
      //DEBUG_PRINT("Skip OP\n");
      break;
    }
  } while( !vm->flag_preemption );
  
  vm->flag_preemption = 0;
  
  return ret;
}

