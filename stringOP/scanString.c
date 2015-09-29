#include "scanString.h" 

#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>

int sscan(const char *str, const char *fmt, ...)
{
  int ret;
  va_list ap;
  char *format_ptr = (char*)fmt;
  char *str_ptr = (char*)str;

  int *p_int;
  long *p_long;

  ret = 0;

  va_start (ap, fmt);

  while ((*format_ptr != 0x0) && (*str_ptr != 0x0)){
    if (*format_ptr == '%'){
      format_ptr++;

      if (*format_ptr != 0x0)
      {
        switch (*format_ptr)
        {
          case 'd':       // expect an int
          case 'u':
            p_int = va_arg( ap, int *);
            if (*str_ptr==0x0) return ret; 
            *p_int = (int)strtol (str_ptr, &str_ptr, 10);
            ret ++;
            break;
          case 'l':       // expect a long
          case 'n':
            p_long = va_arg( ap, long *);
            if (*str_ptr==0x0) return ret;
            *p_long = strtol (str_ptr, &str_ptr, 10);
            ret ++;
            break;
          case 'x':       // expect an int in hexadecimal format
            p_int = va_arg( ap, int *);
            if (*str_ptr==0x0) return ret;
            *p_int = (int)strtol (str_ptr, &str_ptr, 16);
            ret ++;
            break;
          case 'X':  // expect a long in hexadecimal format       
            p_long = va_arg( ap, long *);
            if (*str_ptr==0x0) return ret;
            *p_long = strtol (str_ptr, &str_ptr, 16);
            ret ++;
            break;
        }
      }
    }else{
      if(*format_ptr!=*(str_ptr++)){
        return ret;
      } 
    }
    format_ptr++;
  }       

  return ret;
}
