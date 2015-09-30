/*-
 * trees.h - Tree structure 
 *
 * Copyright 2005 Wenwen Chen
 *
*/

/*!
 * @defgroup    tree_structure Tree structure
 *
 * \brief The definitions of the terms, which are used to describe the tree structure, can be find in http://xlinux.nist.gov/dads//HTML/tree.html \cite tree
 *
 * @{
 *
 * @file
 * @brief       Header definitions for the tree structure
 *
 * @author      Wenwen Chen 
 */
#ifndef __TREE_H__
#define __TREE_H__

#include "treeedges.h"
#include "treenodes.h"

#ifdef  __cplusplus
extern "C" {
#endif
  /*!
   * \brief Fill a path of tree with the given nodes and edges
   *
   *        For example, a path of length 2 consists of the following tree elements: 
   * \verbatim
           nodes[0]  ----- level 0
             /    
            / edges[0]
           /
          V
     nodes[1]        ----- level 1
    \endverbatim
   *
   * \param   p_nodes  Pointer to the nodes
   *          p_edges  Pointer to the edges
   *          len      Length of the tree path, which is equal to the number of nodes or number of edges - 1
   *          flag     Whether update nodes, if the nodes is not empty (node.size != 0)
   *
   * \return           Pointer to the last node 
   */
  inline tree_node *  (__attribute__((always_inline))fillNodes)(tree_node * p_nodes, tree_edge *p_edges, unsigned int len, unsigned int flag){
    if(p_nodes == NULL || p_edges == NULL){
      return NULL;
    }
    int i = 0;
    for(; i<len-1; i++){
      if(flag || p_nodes[i+1].size == 0){
        p_edges[i].func(p_nodes+i, p_edges+i, p_nodes+i+1);
      }
    }
    return p_nodes+i;
  }

#ifdef  __cplusplus
}
#endif /* __cplusplus */

#endif /* __TREE_H__ */

/*!
 * @}
 */
