#ifndef MRBC_SRC_VM_H_
#define MRBC_SRC_VM_H_

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef enum {
  /* internal use */
  MRB_TT_HANDLE = -1,
  /* primitive */
  MRB_TT_EMPTY = 0,
  MRB_TT_NIL,
  MRB_TT_FALSE,		// (note) true/false threshold. see op_jmpif

  MRB_TT_TRUE,
  MRB_TT_FIXNUM,
  MRB_TT_FLOAT,
  MRB_TT_SYMBOL,
  MRB_TT_CLASS,

  /* non-primitive */
  MRB_TT_OBJECT = 20,
  MRB_TT_PROC,
  MRB_TT_ARRAY,
  MRB_TT_STRING,
  MRB_TT_RANGE,
  MRB_TT_HASH,

} mrb_vtype;

typedef int16_t mrb_sym;
typedef struct RObject {
  uint8_t tt : 8;
  union {
    int32_t i;
    const char *str;
  };
} mrb_object;

typedef struct RObject mrb_value;


typedef struct RClass {
  mrb_sym sym_id;	// class name
#if 0
  struct RClass *super;	// mrbc_class[super]
  struct RProc *procs;	// mrbc_proc[rprocs], linked list
#endif
} mrb_class;

//================================================================
/*!@brief
  IREP Internal REPresentation
*/
typedef struct IREP {
  uint16_t nlocals;		//!< # of local variables
  uint16_t nregs;		//!< # of register variables
  uint16_t rlen;		//!< # of child IREP blocks
  uint16_t ilen;		//!< # of iSeq
  uint16_t plen;		//!< # of pool

  uint8_t     *code;		//!< ISEQ (code) BLOCK
  mrb_object  **pools;          //!< array of POOL objects pointer.
  uint8_t     *ptr_to_sym;
  struct IREP **reps;		//!< array of child IREP's pointer.

  //for output
  uint8_t  *pool_head;
  uint16_t pool_total_len;
  uint16_t slen;
  
} mrb_irep;

//================================================================
/*!@brief
  Virtual Machine
*/
typedef struct OVM {
  mrb_irep *irep;
  const uint8_t *mrb;   // bytecode
  mrb_irep *pc_irep;    // PC
  uint16_t  pc;         // PC
  int32_t error_code;
} mrb_vm;



//================================================================
/*!@brief
  Get 32bit value from memory big endian.

  @param  s	Pointer of memory.
  @return	32bit unsigned value.
*/
inline static uint32_t bin_to_uint32( const void *s )
{
#if MRBC_REQUIRE_32BIT_ALIGNMENT
  uint8_t *p = (uint8_t *)s;
  uint32_t x = *p++;
  x <<= 8;
  x |= *p++;
  x <<= 8;
  x |= *p++;
  x <<= 8;
  x |= *p;
  return x;
#else
  uint32_t x = *((uint32_t *)s);
  return (x << 24) | ((x & 0xff00) << 8) | ((x >> 8) & 0xff00) | (x >> 24);
#endif
}


//================================================================
/*!@brief
  Get 16bit value from memory big endian.

  @param  s	Pointer of memory.
  @return	16bit unsigned value.
*/
inline static uint16_t bin_to_uint16( const void *s )
{
#if MRBC_REQUIRE_32BIT_ALIGNMENT
  uint8_t *p = (uint8_t *)s;
  uint16_t x = *p++ << 8;
  x |= *p;
  return x;
#else
  uint16_t x = *((uint16_t *)s);
  return (x << 8) | (x >> 8);
#endif
}

enum OPCODE {
  OP_NOP       = 0x00,
  OP_MOVE      = 0x01,
  OP_LOADL     = 0x02,
  OP_LOADI     = 0x03,
  OP_LOADSYM   = 0x04,
  OP_LOADNIL   = 0x05,
  OP_LOADSELF  = 0x06,
  OP_LOADT     = 0x07,
  OP_LOADF     = 0x08,
  OP_GETGLOBAL = 0x09,
  OP_SETGLOBAL = 0x0a,

  OP_GETIV     = 0x0d,
  OP_SETIV     = 0x0e,

  OP_GETCONST  = 0x11,
  OP_SETCONST  = 0x12,

  OP_GETUPVAR  = 0x15,
  OP_SETUPVAR  = 0x16,
  OP_JMP       = 0x17,
  OP_JMPIF     = 0x18,
  OP_JMPNOT    = 0x19,
  OP_SEND      = 0x20,
  OP_SENDB     = 0x21,

  OP_CALL      = 0x23,

  OP_ENTER     = 0x26,

  OP_RETURN    = 0x29,

  OP_BLKPUSH   = 0x2b,
  OP_ADD       = 0x2c,
  OP_ADDI      = 0x2d,
  OP_SUB       = 0x2e,
  OP_SUBI      = 0x2f,
  OP_MUL       = 0x30,
  OP_DIV       = 0x31,
  OP_EQ        = 0x32,
  OP_LT        = 0x33,
  OP_LE        = 0x34,
  OP_GT        = 0x35,
  OP_GE        = 0x36,
  OP_ARRAY     = 0x37,

  OP_STRING    = 0x3d,
  OP_STRCAT    = 0x3e,
  OP_HASH      = 0x3f,
  OP_LAMBDA    = 0x40,
  OP_RANGE     = 0x41,

  OP_CLASS     = 0x43,

  OP_EXEC      = 0x45,
  OP_METHOD    = 0x46,

  OP_TCLASS    = 0x48,

  OP_STOP      = 0x4a,

  OP_ABORT     = 0x4b,
};

#define LOAD_FILE_IREP_ERROR_ALLOCATION (-1)
#define LOAD_FILE_HEADER_ERROR_MATZ (-1)
#define LOAD_FILE_HEADER_ERROR_VERSION (-1)
#define LOAD_FILE_IREP_ERROR_VERSION (-1)

#define GET_OPCODE(code)            ((code) & 0x7f)
#define GETARG_A(code)              (((code) >> 23) & 0x1ff)
#define GETARG_B(code)              (((code) >> 14) & 0x1ff)
#define GETARG_C(code)              (((code) >>  7) & 0x7f)
#define GETARG_Ax(code)             (((code) >>  7) & 0x1ffffff)

#define GETARG_Bz(code)              GETARG_UNPACK_b(code,14,2)

#define GETARG_UNPACK_b(i,n1,n2)    ((((code)) >> (7+(n2))) & (((1<<(n1))-1)))

#define MKOPCODE(op)                ((op & 0x7f)<<24)
#define MKARG_A(c)                  ((c & 0xff)<<1 | (c & 0x01)>>8)


#define MAXARG_Bx                   (0xffff)
#define MAXARG_sBx                  (MAXARG_Bx>>1)
#define GETARG_Bx(code)             (((code) >>  7) & 0xffff)
#define GETARG_sBx(code)            (GETARG_Bx(code)-MAXARG_sBx)
#define GETARG_C(code)              (((code) >>  7) & 0x7f)


#ifdef __cplusplus
}
#endif
#endif

