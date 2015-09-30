/*-
 * treenodes.c - Tree structure for nodes
 *
 * Copyright 2005 Wenwen Chen
*/

/*!
 * @addtogroup    tree_structure
 * @{
 *
 * @file
 * @brief       Implementation of the tree structure of nodes
 *
 * @author      Wenwen Chen 
 */
#include "treenodes.h"

#if(CACHED_NODES_LEN>0)
tree_node cached_nodes[CACHED_NODES_LEN] = {0};
#endif

tree_node path_nodes[TREE_HEIGTH+1] = {0};

/*!
 * @}
 */

