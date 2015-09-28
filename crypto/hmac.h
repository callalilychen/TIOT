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

unsigned char* hmac(const struct hash_function_construction* hash, const void* key, int key_size, const unsigned char* message, int message_size, unsigned char* res, unsigned int * res_size);
#ifdef  __cplusplus
}
#endif /* __cplusplus */

#endif /* __HMAC_H__ */

