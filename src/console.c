/*! @file
  @brief
  console output module. (not yet input)

  <pre>
  Copyright (C) 2015-2017 Kyushu Institute of Technology.
  Copyright (C) 2015-2017 Shimane IT Open-Innovation Center.

  This file is distributed under BSD 3-Clause License.

  </pre>
*/

#include "vm_config.h"
#include <stdio.h>
#include <stdint.h>
#include <stdarg.h>
#include <string.h>
#include "console.h"



//================================================================
/*! output formatted string

  @param  fstr		format string.
*/
void console_printf(const char *fstr, ...)
{
  va_list ap;
  va_start(ap, fstr);

  char buf[CONSOLE_BUFF_SIZE];
  vsprintf(buf,fstr,ap);
  hal_write_string(buf);
  va_end(ap);
  
}



//================================================================
/*! sprintf subcontract function

  @param  pf	pointer to mrb_printf
  @retval 0	(format string) done.
  @retval 1	found a format identifier.
  @retval -1	buffer full.
  @note		not terminate ('\0') buffer tail.
*/
int mrbc_printf_main( mrb_printf *pf )
{
#if 0
  int ch = -1;
  pf->fmt = (struct RPrintfFormat){0};

  while( pf->p < pf->buf_end && (ch = *pf->fstr) != '\0' ) {
    pf->fstr++;
    if( ch == '%' ) {
      if( *pf->fstr == '%' ) {	// is "%%"
	pf->fstr++;
      } else {
	goto PARSE_FLAG;
      }
    }
    *pf->p++ = ch;
  }
  return -(ch != '\0');


 PARSE_FLAG:
  // parse format - '%' [flag] [width] [.precision] type
  //   e.g. "%05d"
  while( (ch = *pf->fstr) ) {
    switch( ch ) {
    case '+': pf->fmt.flag_plus = 1; break;
    case ' ': pf->fmt.flag_space = 1; break;
    case '-': pf->fmt.flag_minus = 1; break;
    case '0': pf->fmt.flag_zero = 1; break;
    default : goto PARSE_WIDTH;
    }
    pf->fstr++;
  }

 PARSE_WIDTH:
  while( (ch = *pf->fstr - '0'), (0 <= ch && ch <= 9)) {	// isdigit()
    pf->fmt.width = pf->fmt.width * 10 + ch;
    pf->fstr++;
  }
  if( *pf->fstr == '.' ) {
    pf->fstr++;
    while( (ch = *pf->fstr - '0'), (0 <= ch && ch <= 9)) {
      pf->fmt.precision = pf->fmt.precision * 10 + ch;
      pf->fstr++;
    }
  }
  if( *pf->fstr ) pf->fmt.type = *pf->fstr++;

  return 1;
#endif
  return 1;
}



//================================================================
/*! sprintf subcontract function for char '%c'

  @param  pf	pointer to mrb_printf
  @param  ch	output character (ASCII)
  @retval 0	done.
  @retval -1	buffer full.
  @note		not terminate ('\0') buffer tail.
*/
int mrbc_printf_char( mrb_printf *pf, int ch )
{
#if 0
  if( pf->fmt.flag_minus ) {
    if( pf->p == pf->buf_end ) return -1;
    *pf->p++ = ch;
  }

  int width = pf->fmt.width;
  while( --width > 0 ) {
    if( pf->p == pf->buf_end ) return -1;
    *pf->p++ = ' ';
  }

  if( !pf->fmt.flag_minus ) {
    if( pf->p == pf->buf_end ) return -1;
    *pf->p++ = ch;
  }

  return 0;
#endif
  return 0;
}



//================================================================
/*! sprintf subcontract function for char '%s'

  @param  pf	pointer to mrb_printf.
  @param  str	output string.
  @param  pad	padding character.
  @retval 0	done.
  @retval -1	buffer full.
  @note		not terminate ('\0') buffer tail.
*/
int mrbc_printf_str( mrb_printf *pf, const char *str, int pad )
{
#if 0
  int ret = 0;

  if( str == NULL ) str = "(null)";
  int len = strlen(str);
  if( pf->fmt.precision && len > pf->fmt.precision ) len = pf->fmt.precision;

  int tw = len;
  if( pf->fmt.width > len ) tw = pf->fmt.width;

  int remain = pf->buf_end - pf->p;
  if( len > remain ) {
    len = remain;
    ret = -1;
  }
  if( tw > remain ) {
    tw = remain;
    ret = -1;
  }

  int n_pad = tw - len;

  if( !pf->fmt.flag_minus ) {
    while( n_pad-- > 0 ) {
      *pf->p++ = pad;
    }
  }
  while( len-- > 0 ) {
    *pf->p++ = *str++;
  }
  while( n_pad-- > 0 ) {
    *pf->p++ = pad;
  }

  return ret;
#endif
  return 1;
}



//================================================================
/*! sprintf subcontract function for integer '%d' '%x' '%b'

  @param  pf	pointer to mrb_printf.
  @param  value	output value.
  @param  base	n base.
  @retval 0	done.
  @retval -1	buffer full.
  @note		not terminate ('\0') buffer tail.
*/
int mrbc_printf_int( mrb_printf *pf, int32_t value, int base )
{
#if 0
  int sign = 0;
  uint32_t v = (uint32_t)value;

  if( pf->fmt.type == 'd' || pf->fmt.type == 'i' ) {	// signed.
    if( value < 0 ) {
      sign = '-';
      v = (uint32_t)-value;
    } else if( pf->fmt.flag_plus ) {
      sign = '+';
    } else if( pf->fmt.flag_space ) {
      sign = ' ';
    }
  }

  if( pf->fmt.flag_minus || pf->fmt.width == 0 ) {
    pf->fmt.flag_zero = 0; // disable zero padding if left align or width zero.
  }
  pf->fmt.precision = 0;

  int bias_a = (pf->fmt.type == 'X') ? 'A' - 10 : 'a' - 10;

  // create string to local buffer
  char buf[32+2];	// int32 + terminate + 1
  char *p = buf + sizeof(buf) - 1;
  *p = '\0';
  do {
    int i = v % base;
    *--p = (i < 10)? i + '0' : i + bias_a;
    v /= base;
  } while( v != 0 );

  // decide pad character and output sign character
  int pad;
  if( pf->fmt.flag_zero ) {
    pad = '0';
    if( sign ) {
      *pf->p++ = sign;
      if( pf->p >= pf->buf_end ) return -1;
      pf->fmt.width--;
    }
  } else {
    pad = ' ';
    if( sign ) *--p = sign;
  }
  return mrbc_printf_str( pf, p, pad );
#endif
  return 1;
}




//================================================================
/*! replace output buffer

  @param  pf	pointer to mrb_printf
  @param  buf	pointer to output buffer.
  @param  size	buffer size.
*/
void mrbc_printf_replace_buffer(mrb_printf *pf, char *buf, int size)
{
  int p_ofs = pf->p - pf->buf;
  pf->buf = buf;
  pf->buf_end = buf + size - 1;
  pf->p = pf->buf + p_ofs;
}

