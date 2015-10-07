/*-
 * treeedges.c - Tree structure for edges 
 *
 * Copyright 2015 Wenwen Chen
*/

/*!
 * \addtogroup    tree_structure
 * \{
 *
 * \file
 * \brief       Implementation of the tree structure of edges
 *
 * \author      Wenwen Chen 
 */
#include "treeedges.h"
#include <string.h>
#include "utils.h"

tree_edge path_edges[TREE_HEIGTH]={0};

void edgeFunc(tree_node *p_parent_node, tree_edge *p_edge, tree_node *p_node)
{
  unsigned int size = 0;

  if(p_edge->params_size>0){
    while(size+(p_edge->params_size) < p_parent_node->size){
      memcpy(p_node->block + size, p_edge->params, (size_t)(p_edge->params_size));
      size+=(p_edge->params_size);
    }
  }
  memcpy(p_node->block + size, p_edge->params,(size_t)(p_parent_node->size - size));
  optimizedXOR(p_node->block, p_parent_node->block, (size_t)(p_parent_node->size), p_node->block);
  HASH_FUNC(p_node->block, (size_t)(p_parent_node->size), p_node->block);
  p_node->size = NODE_SIZE; 
}
/*!
 * \}
 */
