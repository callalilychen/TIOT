#ifndef __TREE_EDGES_H__
#define __TREE_EDGES_H__

#include "treeconfig.h"
#include "treenodes.h"

#ifdef  __cplusplus
extern "C" {
#endif


  typedef struct tree_edge{
    void (*func)(tree_node *, struct tree_edge *, tree_node *);
    unsigned char * params;
    unsigned int params_size;
  }tree_edge;
  
  extern tree_edge path_edges[MAX_LEVEL-1];

  inline tree_edge * (__attribute__((always_inline))getEdges)(unsigned int level){
    if(level+1 > MAX_LEVEL){
      return NULL;
    }
    return path_edges;  
  }

  void edgeFunc(tree_node *, tree_edge *edge, tree_node *);

  // TODO inline tree_edge * getEdgesWithFunction(unsigned int);

#ifdef  __cplusplus
}
#endif /* __cplusplus */

#endif /* __TREE_EDGES_H__ */

