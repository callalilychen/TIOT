#ifndef __INTERFACE_H__
#define __INTERFACE_H__
#include <stdint.h>
#include <stdio.h>
#include "tree.h"
#include "hmac.h"

#ifdef  __cplusplus
extern "C" {
#endif

#define PRINT(...) printf(__VA_ARGS__)

#define HASH_BLOCK_LENGTH SHA256_DIGEST_LENGTH
//#define HASH_BLOCK_LENGTH SHA256_DIGEST_LENGTH
#define HASH_FUNC hashfunction
#define SUCC 0

#ifdef  __cplusplus
}
#endif /* __cplusplus */

#endif /* __INTERFACE_H__ */

