#ifndef __TREE_CONFIG_H__
#define __TREE_CONFIG_H__

#include "interface.h"

#ifdef  __cplusplus
extern "C" {
#endif
#define NODE_SIZE HASH_SIZE
#define HASH_FUNC sha_construction.func

#define MAX_LEVEL 3
#define MAX_CACHED_NODES 0
#define MAX_VALID_STATES 0xff
#if(MAX_VALID_STATES & 0x7)
#define BIT_MAP_SIZE MAX_VALID_STATES>>3
#elif
#define BIT_MAP_SIZE (MAX_VALID_STATES>>3) + 1
#endif


#ifdef  __cplusplus
}
#endif /* __cplusplus */

#endif /* __TREE_CONFIG_H__ */

