#ifndef PRINT_STRING_H
#define PRINT_STRING_H
/*****************************************************************************/
/* Include files                                                             */
/*****************************************************************************/
#ifdef  __cplusplus
extern "C" {
#endif

int * getHandler(void);
void setHandler(int (*)(char));
void print(char *, ...);
int sprint(char* str, char *format, ...);

#ifdef  __cplusplus
}
#endif /* __cplusplus */
#endif
