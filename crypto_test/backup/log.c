#include "log.h"
inline void log_init(void){
#if defined(DEBUG) || defined(INFO)
  setHandler(&uart_putByte);
#endif
}
inline void info(char *format, ...){
#ifdef INFO
	va_list a;
	va_start(a, format);
  print(format, a);
  va_end(a );
#endif
}
inline void debug(char *format, ...){
#ifdef DEBUG
	va_list a;
	va_start(a, format);
  print(format, a);
  va_end(a );
#endif
}

