/*-
 * treenodes.h - Tree structure for nodes
 *
 * Copyright 2005 Wenwen Chen
*/
/*!
 * \addtogroup    tree_structure
 * \{
 *
 * \file
 * \brief       Header definitions for the tree structure of nodes
 *
 * \author      Wenwen Chen 
 */
#ifndef __TREE_NODES_H__
#define __TREE_NODES_H__

#include <string.h>
#include "treeconfig.h"
#ifdef  __cplusplus
extern "C" {
#endif

  /*!
   * \brief Tree node structure
   *        
   *        A Tree node consists of a block of bytes and the size of the block
   */
  typedef struct tree_node{
    unsigned char block[NODE_SIZE];
    unsigned int size;
  }tree_node;

  /*!
   * \brief Static memory allocation for nodes of a tree path 
   *
   *        TREE_HEIGTH + 1 nodes are allocated. At the beginning, all values of edges will be set to zero 
   */
  extern tree_node path_nodes[TREE_HEIGTH+1];

#if(CACHED_NODES_LEN>0)
  /*!
   * \brief Static memory allocation of CACHED_NODES_LEN nodes for cache usage
   *
   *        At the beginning, all values of edges will be set to zero 
   */
  extern tree_node cached_nodes[CACHED_NODES_LEN];
#endif
  
  /*!
   * \brief Set value to the tree root
   *
   *        Tree root is the first node of path_nodes
   *
   * \param   value  The new tree root value
   *          size   Size of the value in byte 
   *
   * \return         Nonce
   */
  inline void (__attribute__((always_inline))setRoot)(unsigned char * value, unsigned int size)
  {
    if(size > NODE_SIZE){
      sha_construction.func(value, size, path_nodes->block);
      path_nodes->size = sha_construction.size;
    } else {
      memcpy(path_nodes->block, value, size);
      path_nodes->size = size;
    }
  }

#if(CACHED_NODES_LEN>0)
  /*!
   * \brief Set value to a cached tree node
   *
   * \param   index  The index of the to be set cached node in cached_nodes
   *          value  The new cached root value
   *          size   Size of the value in byte 
   *
   * \return         Nonce
   */
  inline void (__attribute__((always_inline))setCachedNode)(unsigned int index, unsigned char * value, unsigned int size)
  {
    if(index < CACHED_NODES_LEN)
    {
      if(size > NODE_SIZE){
        sha_construction.func(value, size, cached_nodes[index].block);
        cached_nodes[index].size = sha_construction.size;
      } else {
        memcpy(cached_nodes[index].block, value, size);
        cached_nodes[index].size = size;
      }
    }
  }
#endif

  /*!
   * \brief Get value of the tree root
   *
   *        Tree root is the first node of path_nodes
   *
   * \return        Pointer to the tree root 
   */
  inline tree_node *(__attribute__((always_inline))getRoot)(void){
    return path_nodes;
  }

  /*!
   * \brief Get the path of tree nodes to a given depth
   *
   *        Except the tree 
   *
   * \param   depth  The depth of the last tree node in the path
   *
   * \return         Pinter to the tree path or NULL, if depth is greater then TREE_HEIGTH 
   */
  inline tree_node * (__attribute__((always_inline))getPathFromRoot)(uint8_t depth){
    if(depth > TREE_HEIGTH){
      return NULL;
    }
    for(int i = 1; i<= depth; i++){
      path_nodes[i].size = 0;
    }
    return path_nodes;  
  }

#if(CACHED_NODES_LEN>0)
  /*!
   * \brief Get the path of tree nodes to a given depth using the cached node of a given index
   *
   *        Except the tree 
   *
   * \param   depth  The depth of the last tree node in the path
   *          index  The index of the to be set cached node in cached_nodes
   *
   * \return         Pinter to the tree path or NULL, if depth is greater then TREE_HEIGTH 
   */
  inline tree_node* (__attribute__((always_inline))getPathFromRootThenCachedNodes)(unsigned int depth, unsigned int index){
    if(depth > TREE_HEIGTH){
      return NULL;
    }
    int i = 1;
    if(index < CACHED_NODES_LEN && cached_nodes[index].size>0){
      memcpy(path_nodes+(i++), cached_nodes + index, sizeof(tree_node));
    }
    for(; i<= depth; i++){
      path_nodes[i].size = 0;
    }
    return path_nodes;  
  }

  /*!
   * \brief Get the path of tree nodes to a given depth from the cached node of a given index
   *
   *        Except the tree 
   *
   * \param   depth  The depth of the last tree node in the path
   *          index  The index of the to be set cached node in cached_nodes
   *
   * \return         Pinter to the tree path or NULL, if depth is greater then TREE_HEIGTH 
   */
  inline tree_node * (__attribute__((always_inline))getPathFromCachedNodes)(unsigned int depth, unsigned int index){
    if(depth > TREE_HEIGTH){
      return NULL;
    }
    int i = 0;
    if(index < CACHED_NODES_LEN && cached_nodes[index].size>0){
      memcpy(path_nodes, cached_nodes + index, sizeof(tree_node));
      i++;
    }
    for(; i<= depth; i++){
      path_nodes[i].size = 0;
    }
    return path_nodes;  
  }
#endif

#ifdef  __cplusplus
}
#endif /* __cplusplus */

#endif /* __TREE_NODES_H__ */
/*!
 * \}
 */
