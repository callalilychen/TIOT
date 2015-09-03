#include "tree.h"
#include "utils.h"

unsigned char * fill_nodes(tree_node * nodes, tree_edge * edges, unsigned int level, unsigned int flag){
  int i =0;
  for(; i<level; i++){
    if(flag || nodes[i].size == 0){
      edges[i].func(&nodes[i], &edges[i], &nodes[i+1]);
    }
  }
  return nodes[i].block;
}

void * edge_func(tree_node *parent_node, tree_edge *edge, tree_node *node)
{
  unsigned int size = 0;
  unsigned int counter = 0;

  if(edge -> params_counter == 0){
    return;
  }
  while(size+(edge->params_size)[counter] < parent_node->size){
    memcpy(node->block + size, (edge->params)[counter], (size_t)(edge->params_size[counter]));
    size+=(edge->params_size)[counter];
    if((++counter)>=edge-> params_counter){
      counter = 0;
    }
  }
  memcpy(node->block + size, (edge->params)[counter],(size_t)(parent_node->size - size));
  optimizedXOR(node->block, parent_node->block, (size_t)(parent_node->size), node->block);
  HASH_FUNC(node->block, (size_t)(parent_node->size), node->block);
  node->size = NODE_SIZE; 
}
