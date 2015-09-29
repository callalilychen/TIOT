
/******************************************************************************
 *                          Reusable MSP430 printf()
 *
 * Description: This printf function was written by oPossum and originally
 *              posted on the 43oh.com forums. For more information on this
 *              code, please see the link below.
 *
 *              http://www.43oh.com/forum/viewtopic.php?f=10&t=1732
 *
 *              A big thanks to oPossum for sharing such great code!
 *
 * Author:  oPossum
 * Source:  http://www.43oh.com/forum/viewtopic.php?f=10&t=1732
 * Date:    10-17-11
 *
 * Note: This comment section was written by Nicholas J. Conn on 06-07-2012
 *       for use on NJC's MSP430 LaunchPad Blog.
 ******************************************************************************/
#include "printString.h" 
#include <stdint.h>
#include <stdarg.h>

static int nohandle(char c)
{
  // do nothing
}
int (*handleC)(char) = &nohandle;
int * getHandler(void)
{
  return handleC;
}
void setHandler(int (*handle)(char))
{
  handleC = handle;
}
static inline int handleS(const char *s)
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
 
static void xtoa(unsigned long x, const unsigned long *dp) {
	char c;
	unsigned long d;
	if (x) {
		while (x < *dp)
			++dp;
		do {
			d = *dp++;
			c = '0';
			while (x >= d)
				++c, x -= d;
			handleC(c);
		} while (!(d & 1));
	} else
		handleC('0');
}
 
static void puth(unsigned n) {
	static const char hex[16] = { '0', '1', '2', '3', '4', '5', '6', '7', '8',
			'9', 'A', 'B', 'C', 'D', 'E', 'F' };
	handleC(hex[n & 15]);
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
					xtoa((unsigned)i, dv + 5);
				break;
				case 'l':// 32 bit Long
				case 'n':// 32 bit uNsigned loNg
					l = va_arg(a, long);
					if(c == 'l' && l < 0) l = -l, handleC('-');
					xtoa((unsigned long)l, dv);
				break;
        
				case 'w':// 32 bit hexadecimal
					l = va_arg(a, long);
          flag = 1;
          if(l>0xfffffff)
          {
            puth(l >> 28);
          }
          if(l>0xffffff)
          {
            puth(l >> 24);
          }
          if(l>0xfffff)
          {
            puth(l >> 20);
          }
          if(l>0xffff)
          {
            puth(l >> 16);
          }
          i = (int) l;
        case 'x': // 16 bit hexadecimal
          if(!flag){
            i = va_arg(a, int);
          }
          if(i>0xfff)
          {
            puth(i >> 12);
          }
          if(i>0xff)
          {
            puth(i >> 8);
          }
          if(i>0xf)
          {
            puth(i >> 4);
          }
					puth(i);
				break;
				case 0: return;
				default: goto bad_fmt;
			}
		} else{
			bad_fmt: handleC(c);
    }        
    va_end(a );
  }          
}
