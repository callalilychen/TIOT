#ifndef PRINT_STRING_H
#define PRINT_STRING_H
#include "stdarg.h"
void setHandler(int (*)(unsigned char));
void print(char *, ...);
#endif
