#ifndef __HMAC_H__
#define __HMAC_H__

#include <string.h>
#include <strings.h>
#include "interface.h"

#ifdef  __cplusplus
extern "C" {
#endif
#ifndef WORD_SIZE
#define WORD_SIZE 2
#define WORD_TYPE (uint16_t *)
#endif

unsigned char* hmac(const struct hash_function_construction*, const void*, int, const unsigned char*,int , unsigned char* , unsigned int *);
#ifdef  __cplusplus
}
#endif /* __cplusplus */

#endif /* __HMAC_H__ */

