#ifndef __TREE_NODES_H__
#define __TREE_NODES_H__

#include "treeconfig.h"
#include "string.h"
#include "strings.h"
#ifdef  __cplusplus
extern "C" {
#endif


  typedef struct tree_node{
    unsigned char block[NODE_SIZE];
    unsigned int size;
  }tree_node;

  extern tree_node path[MAX_LEVEL];
  extern tree_node cached_nodes[MAX_CACHED_NODES];

  inline void (__attribute__((always_inline))setRoot)(unsigned char * item, unsigned int len)
  {
    if(len > NODE_SIZE){
      sha_construction.func(item, len, path->block);
      path->size = sha_construction.size;
    } else {
      memcpy(path->block, item, len);
      path->size = len;
    }
  }

  inline tree_node *(__attribute__((always_inline))getRoot)(void){
    return path;
  }

  inline tree_node * (__attribute__((always_inline))getPathFromRoot)(uint8_t level){
    bzero(path+1, (level - 1)*sizeof(tree_node));
    return path;  
  }

  inline tree_node* (__attribute__((always_inline))getPathFromRootWithCachedNodes)(unsigned int level, unsigned int index){
    if(level > MAX_LEVEL){
      return NULL;
    }
    int i = 1;
    if(index < MAX_CACHED_NODES && cached_nodes[index].size>0){
      memcpy(path+(i++), cached_nodes + index, sizeof(tree_node));
    }
    bzero(path+i, (level - i)*sizeof(tree_node));
    return path;  
  }

  inline tree_node * (__attribute__((always_inline))getPathFromCachedNodes)(unsigned int level, unsigned int index){
    return getPathFromRootWithCachedNodes(level+1,index)+1;  
  }

#ifdef  __cplusplus
}
#endif /* __cplusplus */

#endif /* __TREE_NODES_H__ */

