#ifndef SCAN_STRING_H
#define SCAN_STRING_H

/*****************************************************************************/
/* Include files                                                             */
/*****************************************************************************/
#ifdef  __cplusplus
extern "C" {
#endif
#ifdef MSP
#define SCAN(...)  
#define SSCAN(...) sscan(__VA_ARGS__)  
#else
#define SCAN(...)  scanf(__VA_ARGS__)
#define SSCAN(...) sscanf(__VA_ARGS__)  
#endif
  int sscan(const char *str, const char *fmt, ...);

#ifdef  __cplusplus
}
#endif /* __cplusplus */

#endif /* __SCAN_STRING_H__ */

