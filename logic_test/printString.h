#ifndef PRINT_STRING_H
#define PRINT_STRING_H
#include <stdarg.h>
int * getHandler(void);
void setHandler(int (*)(char));
void print(char *, ...);
#endif
