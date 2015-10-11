
/*!
 * \brief Simple print funktions
 *
 * \note This is only test code for MSP430 microcontroller, on which the standard print functions does not work well. 
 *       The code contains severay bugs and may cause memory leak
 *
 * The code is based on an implementation from http://www.43oh.com/forum/viewtopic.php?f=10&t=1732
 *
 * Author:  oPossum
 * Date:    10-17-11
 *
 * Note: This comment section was written by Nicholas J. Conn on 06-07-2012
 *       for use on NJC's MSP430 LaunchPad Blog.
 */
#include "printString.h" 
#include <stdint.h>
#include <stdarg.h>
#include <string.h>

static int nohandle(char c)
{
  // do nothing
  return 0;
}
int (*handleC)(char) = &nohandle;
int * getHandler(void)
{
  return (int *)handleC;
}
void setHandler(int (*handle)(char))
{
  handleC = handle;
}
static inline void handleS(const char *s)
{
	char c;
 
	// Loops through each character in string 's'
	while (c = *s++) {
		handleC(c);
	}
}

static const unsigned long dv[] = {
//  4294967296      // 32 bit unsigned max
		1000000000,// +0
		100000000, // +1
		10000000, // +2
		1000000, // +3
		100000, // +4
//  65535      // 16 bit unsigned max
		10000, // +5
		1000, // +6
		100, // +7
		10, // +8
		1, // +9
		};
 
static int xtoa(int (*printC)(char), unsigned long x, const unsigned long *dp) {
	char c;
	unsigned long d;
  int res = 0;
	if (x) {
		while (x < *dp)
			++dp;
		do {
			d = *dp++;
			c = '0';
			while (x >= d)
				++c, x -= d;
			printC(c);
      res++;
		} while (!(d & 1));
	} else{
		printC('0');
    res++;
  }
  return res;
}
 
static int puth(int (*printC)(char),unsigned n) {
	static const char hex[16] = { '0', '1', '2', '3', '4', '5', '6', '7', '8',
			'9', 'a', 'b', 'c', 'd', 'e', 'f' };
	printC(hex[n & 15]);
  return 1;
}
 void print(char *format, ...)
{
	char c;
	int i;
	long l;
  unsigned short flag = 0;

 
	va_list a;
	va_start(a, format);
	while(c = *format++) {
    flag = 0;
		if(c == '%') {
			switch(c = *format++) {
				case 's': // String
					handleS(va_arg(a, char*));
					break;
				case 'c':// Char
					handleC((char)va_arg(a, int));
				break;
				case 'd':// 16 bit Integer
				case 'u':// 16 bit Unsigned
					i = va_arg(a, int);
					if(c == 'i' && i < 0) i = -i, handleC('-');
					xtoa(handleC, (unsigned)i, dv + 5);
				break;
				case 'l':// 32 bit Long
				case 'n':// 32 bit uNsigned loNg
					l = va_arg(a, long);
					if(c == 'l' && l < 0) l = -l, handleC('-');
					xtoa(handleC, (unsigned long)l, dv);
				break;
        
				case 'w':// 32 bit hexadecimal
					l = va_arg(a, long);
          flag = 1;
          if(l>0xfffffff)
          {
            puth(handleC, l >> 28);
          }
          if(l>0xffffff)
          {
            puth(handleC, l >> 24);
          }
          if(l>0xfffff)
          {
            puth(handleC, l >> 20);
          }
          if(l>0xffff)
          {
            puth(handleC, l >> 16);
          }
          i = (int) l;
        case 'x': // 16 bit hexadecimal
          if(!flag){
            i = va_arg(a, int);
          }
          if(i>0xfff)
          {
            puth(handleC, i >> 12);
          }
          if(i>0xff)
          {
            puth(handleC, i >> 8);
          }
          if(i>0xf)
          {
            puth(handleC, i >> 4);
          }
					puth(handleC, i);
				break;
				case 0: return;
				default: goto bad_fmt;
			}
		} else{
			bad_fmt: handleC(c);
    }        
  }          
    va_end(a );
}

char * curr_str = 0;
static int putctostr( char c){
  *(curr_str++) = c;
  return 1;
}

int sprint(char* str, char *format, ...)
{
	char c;
	int i;
	long l;
  unsigned short flag = 0;
  int len = 0;
  if(0==str){
    return -1;
  }
  curr_str = str;

  char * s;
  unsigned int s_len;
 
	va_list a;
	va_start(a, format);
	while(c = *format++) {
    flag = 0;
		if(c == '%') {
			switch(c = *format++) {
				case 's': // String
          s = (char*)va_arg(a, char*);
          s_len = strlen(s);
          memcpy(curr_str, s, s_len);
          curr_str+=s_len;
					len += s_len;
					break;
				case 'c':// Char
					len += putctostr((char)va_arg(a, int));
				break;
				case 'd':// 16 bit Integer
				case 'u':// 16 bit Unsigned
					i = va_arg(a, int);
					if(c == 'i' && i < 0){
            i = -i;
            len += putctostr('-');
          }
					len+=xtoa(putctostr, (unsigned)i, dv + 5);
				break;
				case 'l':// 32 bit Long
				case 'n':// 32 bit uNsigned loNg
					l = va_arg(a, long);
					if(c == 'l' && l < 0){
            l = -l;
            len += putctostr('-');
          }
					len += xtoa(putctostr,(unsigned long)l, dv);
				break;
        
				case 'w':// 32 bit hexadecimal
					l = va_arg(a, long);
          flag = 1;
          if(l>0xfffffff)
          {
            len += puth(putctostr,l >> 28);
          }
          if(l>0xffffff)
          {
            len += puth(putctostr,l >> 24);
          }
          if(l>0xfffff)
          {
            len += puth(putctostr,l >> 20);
          }
          if(l>0xffff)
          {
            len += puth(putctostr,l >> 16);
          }
          i = (int) l;
        case 'x': // 16 bit hexadecimal
          if(!flag){
            i = va_arg(a, int);
          }
          if(i>0xfff)
          {
            len += puth(putctostr,i >> 12);
          }
          if(i>0xff)
          {
            len += puth(putctostr,i >> 8);
          }
          if(i>0xf)
          {
            len += puth(putctostr,i >> 4);
          }
					len += puth(putctostr, i);
				  break;
				case 0: 
          goto end;
				default: goto bad_fmt;
			}
		} else{
			bad_fmt: len += putctostr(c);
    }        
  }          
end:
  va_end(a );
  *curr_str = 0;
  curr_str = 0;
  return len;
}
