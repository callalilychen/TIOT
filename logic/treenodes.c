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

tree_node cached_nodes[CACHED_NODES_LEN] = {0};

tree_node path_nodes[MAX_LEVEL+1] = {0};

/*!
 * @}
 */

