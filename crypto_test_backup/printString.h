#ifndef PRINT_STRING_H
#define PRINT_STRING_H
#include "stdarg.h"
void setHandler(int (*)(char));
void print(char *, ...);
#endif
