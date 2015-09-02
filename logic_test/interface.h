#ifndef __INTERFACE_H__
#define __INTERFACE_H__
#include <stdint.h>
#include <stdio.h>
#include "tree.h"
#include "hmac.h"
#include "utils.h"

#ifdef  __cplusplus
extern "C" {
#endif

#define PRINT(...) printf(__VA_ARGS__)

#define HASH_BLOCK_LENGTH SHA256_DIGEST_LENGTH
#define HASH_FUNC hashfunction
#define MAC_FUNC hmac
#define EDGE_FUNC edge
#define SUCC 0
#define FAIL -1

#define H2N(x, y, size) if(size == 2) x = sl_Hons(y);\
                        else if(size == 4) x = sl_Htonl(y);

#ifdef  __cplusplus
}
#endif /* __cplusplus */

#endif /* __INTERFACE_H__ */

