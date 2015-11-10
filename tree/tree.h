/*-
 * trees.h - Tree structure 
 *
 * Copyright 2015 Wenwen Chen
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
   * \param   p_edges  Pointer to the edges
   * \param   depth    The depth of the last tree node in the path
   * \param   flag     Whether update nodes, if the nodes is not empty (node.size != 0)
   *
   * \return           Pointer to the last node or NULL, if p_nodes is NULL, the first node of p_nodes is empty or p_edges is NULL 
   */
  inline tree_node *  __attribute__((always_inline))fillNodes(tree_node * p_nodes, tree_edge *p_edges, unsigned int depth){
    if(p_nodes == NULL || p_edges == NULL){
      return NULL;
    }
    /*!
     * If the first node of the p_nodes is empty, its child node can not be filled
     */
    if(p_nodes->size == 0){
      return NULL;
    }
    int i = 0;
    for(; i<depth; i++){
      p_edges[i].func(p_nodes+i, p_edges+i, p_nodes+i+1);
      PRINT("Caculate edge:\n");
      printBlock("parent node", p_nodes[i].block, p_nodes[i].size );
      printBlock("edge paramter", p_edges[i].params, p_edges[i].params_size );
      printBlock("child node", p_nodes[i+1].block, p_nodes[i+1].size );
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
