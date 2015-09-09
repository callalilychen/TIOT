#ifndef __INTERFACE_H__
#define __INTERFACE_H__
#include <stdint.h>
#include <stdio.h>
#include "sha256.h"

#ifdef  __cplusplus
extern "C" {
#endif

#define PRINT(...) printf(__VA_ARGS__)

//TODO somewhere else
#define MAX_CLIENTS 0xff
#define MAX_LEVEL 3
#define SUCC 0
#define FAIL -1

#define H2N(x, y, size) if(size == 2) x = sl_Hons(y);\
                        else if(size == 4) x = sl_Htonl(y);

  typedef struct hash_function_construction{
    unsigned char *(*func) (const unsigned char *d, size_t n, unsigned char *md);
    unsigned int block_size;
    unsigned int size;
  }hash_function_construction;

#define HASH_SIZE SHA256_DIGEST_LENGTH
extern const hash_function_construction sha_construction;

#ifdef  __cplusplus
}
#endif /* __cplusplus */

#endif /* __INTERFACE_H__ */

