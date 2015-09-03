#ifndef __TREE_H__
#define __TREE_H__

#include <stdio.h>
#include <string.h>
#include "interface.h"

#ifdef  __cplusplus
extern "C" {
#endif
#define NODE_SIZE HASH_SIZE
#define HASH_FUNC sha256_construction.func

  typedef struct tree_node{
    unsigned char block[NODE_SIZE];
    unsigned int size;
  }tree_node;

  typedef struct tree_edge{
    void (*func)(tree_node *, struct tree_edge *, tree_node *);
    unsigned char ** params;
    unsigned int* params_size;
    unsigned int params_counter;
  }tree_edge;

  unsigned char * fill_nodes(tree_node *, tree_edge*, unsigned int, unsigned int);
  void * edge_func(tree_node *, tree_edge *edge, tree_node *);
#ifdef  __cplusplus
}
#endif /* __cplusplus */

#endif /* __TREE_H__ */

