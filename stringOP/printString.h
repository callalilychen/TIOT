#ifndef PRINT_STRING_H
#define PRINT_STRING_H
/*****************************************************************************/
/* Include files                                                             */
/*****************************************************************************/
#ifdef  __cplusplus
extern "C" {
#endif
#ifdef MSP
#define DEBUG(...) 
#define PRINT(...) 
#define  printf(...) 
#define SPRINT(...) sprint(__VA_ARGS__)
#else
#define DEBUG(...) printf(__VA_ARGS__)
#define PRINT(...) printf(__VA_ARGS__)
#define SPRINT(...) sprintf(__VA_ARGS__)
#endif

int * getHandler(void);
void setHandler(int (*)(char));
void print(char *, ...);
int sprint(char* str, char *format, ...);

#ifdef  __cplusplus
}
#endif /* __cplusplus */
#endif
