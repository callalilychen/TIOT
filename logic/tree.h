#ifndef __TREE_H__
#define __TREE_H__

#include "treeedges.h"
#include "treenodes.h"

#ifdef  __cplusplus
extern "C" {
#endif

  inline tree_node *  (__attribute__((always_inline))fillNodes)(tree_node * nodes, tree_edge *edges, unsigned int level, unsigned int flag){
    if(nodes == NULL || edges == NULL){
      printf("nodes size %d\n", nodes->size);
      return NULL;
    }
    int i = 0;
    for(; i<level-1; i++){
      printf("%d: nodes size %d\n",i, (nodes+i)->size);
      if(flag || nodes[i+1].size == 0){
        edges[i].func(nodes+i, edges+i, nodes+i+1);
      printf("%d: nodes size %d\n",i, (nodes+i)->size);
      }
    }
      printf("2nodes size %d\n", (nodes+i)->size);
    return nodes+i;
  }

#ifdef  __cplusplus
}
#endif /* __cplusplus */

#endif /* __TREE_H__ */
