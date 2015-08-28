#ifndef LOG_H
#define LOG_H

#include "uart.h"
#include "printString.h"
//#define DEBUG
#define INFO

inline void log_init(void);
inline void info(char *, ...);
inline void debug(char *, ...);
#endif
