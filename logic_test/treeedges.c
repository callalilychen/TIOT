#include "treeedges.h"
#include <string.h>
#include "utils.h"

tree_edge path_edges[MAX_LEVEL-1]={0};

void edgeFunc(tree_node *parent_node, tree_edge *edge, tree_node *node)
{
  unsigned int size = 0;

  if(edge->params_size>0){
    while(size+(edge->params_size) < parent_node->size){
      memcpy(node->block + size, edge->params, (size_t)(edge->params_size));
      size+=(edge->params_size);
    }
  }
  memcpy(node->block + size, edge->params,(size_t)(parent_node->size - size));
  optimizedXOR(node->block, parent_node->block, (size_t)(parent_node->size), node->block);
  HASH_FUNC(node->block, (size_t)(parent_node->size), node->block);
  node->size = NODE_SIZE; 
}
