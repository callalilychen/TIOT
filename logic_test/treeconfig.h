#ifndef __TREE_CONFIG_H__
#define __TREE_CONFIG_H__

#include "interface.h"

#ifdef  __cplusplus
extern "C" {
#endif
#define MAX_LEVEL 4
#define NODE_SIZE HASH_SIZE
#define HASH_FUNC sha_construction.func

#define MAX_CACHED_NODES 5

#ifdef  __cplusplus
}
#endif /* __cplusplus */

#endif /* __TREE_CONFIG_H__ */

