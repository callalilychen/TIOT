#ifndef PRINT_STRING_H
#define PRINT_STRING_H
int * getHandler(void);
void setHandler(int (*)(char));
void print(char *, ...);
#endif
