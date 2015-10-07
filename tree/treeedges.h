/*-
 * treeedges.h - Tree structure for edges 
 *
 * Copyright 2015 Wenwen Chen
*/

/*!
 * @addtogroup    tree_structure
 * @{
 *
 * @file
 * @brief       Header definitions for the tree structure of edges
 *
 * @author      Wenwen Chen 
 */
#ifndef __TREE_EDGES_H__
#define __TREE_EDGES_H__

#include "treeconfig.h"
#include "treenodes.h"

#ifdef  __cplusplus
extern "C" {
#endif

  /*!
   * \brief Tree edge structure 
   *      
   *        A tree edge consists of a pointer to a edge function, additional parameters in bytes to pass to the function and the size of this parameters block
   *
   */
  typedef struct tree_edge{
    void (*func)(tree_node *p_parent_node, struct tree_edge *p_edge, tree_node *p_node);
    unsigned char * params;
    unsigned int params_size;
  }tree_edge;
  
  /*!
   * \brief Static memory allocation for edges of a tree path 
   *
   *        At the beginning, all values of edges will be set to zero 
   */
  extern tree_edge path_edges[TREE_HEIGTH];

  /*!
   * \brief Get tree edges with given length
   *
   *        The given length should be not greater than TREE_HEIGTH, because the maximal tree edges is defined by TREE_HEIGTH
   *
   * \param len Length of the wished tree edges
   *
   * \return    A pointer to the tree edges or NULL, if len is greater than predefined TREE_HEIGTH 
   */
  inline tree_edge * __attribute__((always_inline))getEdges(unsigned int len){
    if(len > TREE_HEIGTH){
      return NULL;
    }
    return path_edges;  
  }

  /*!
   * \brief Get tree edges with given length and assign the func data
   *
   *        The given length should be not greater than TREE_HEIGTH, because the maximal tree edges is defined by TREE_HEIGTH
   *
   * \param len   Length of the wished tree edges
   * \param func  The edge function pointer for all edges
   *
   * \return      A pointer to the tree edges or NULL, if len is greater than predefined TREE_HEIGTH 
   */
  inline tree_edge * __attribute__((always_inline)) getEdgesWithFunction(unsigned int len, void (*func)(tree_node *p_parent_node, tree_edge *p_edge, tree_node *p_node)){
    if(len > TREE_HEIGTH){
      return NULL;
    }
    for(int i = 0; i < len; i++){
      path_edges[i].func = func;
    }
    return path_edges;  
  }

  /*!
   * \brief A default edge function
   *
   *        The relationship between the paramaters is shown in the flowing figure
   * \verbatim
          p_parent_node
              |
              | p_edge
              |
              V
            p_node
     \endverbatim
   *        The value of result node is the hash value of parent node value xor repeated edge->params
   * \verbatim
          +-----------+   +------+----+
          |parent ndoe|   |params|para| 
          +-----------+   +------+----+
              |                 |
              +-------xor-------+
                       |
                      hash
                       |
                       V
                +-----------+
                |    ndoe   |
                +-----------+
     \endverbatim
   *
   * \param p_parent_node The parent node of the edge
   * \param p_edge        The edge data
   * \param p_node        The child node of the edge
   *
   * \return            None 
   */
  void edgeFunc(tree_node *p_parent_node, tree_edge *p_edge, tree_node *p_node);


#ifdef  __cplusplus
}
#endif /* __cplusplus */

#endif /* __TREE_EDGES_H__ */
/*!
 * @}
 */
