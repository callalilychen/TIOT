/*-
 * treenodes.h - Tree structure for nodes
 *
 * Copyright 2015 Wenwen Chen
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
   * \brief Initialize reserved tree node storage, set all node size to 0.
   *
   * \return         Nonce
   */
  inline void __attribute__((always_inline))initTreeNodes()
  {
    for(int i = 0; i < TREE_HEIGTH+1; i++){
      path_nodes[i].size = 0;
    }
    for(int i = 0; i < CACHED_NODES_LEN; i++){
      cached_nodes[i].size = 0;
    }
  }

  /*!
   * \brief Set value to the tree root
   *
   *        Tree root is the first node of path_nodes
   *
   * \param   value  The new tree root value
   * \param   size   Size of the value in byte 
   *
   * \return         Nonce
   */
  inline void __attribute__((always_inline))setRoot(unsigned char * value, unsigned int size)
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
   * \param   p_node The new cached node
   *
   * \return         Nonce
   */
  inline void __attribute__((always_inline))setCachedNode(unsigned int index, tree_node * p_node)
  {
    if(index < CACHED_NODES_LEN)
    {
      if(p_node ->size > NODE_SIZE){
        sha_construction.func(p_node->block, p_node->size, cached_nodes[index].block);
        cached_nodes[index].size = sha_construction.size;
      } else {
        memcpy(cached_nodes[index].block, p_node->block, p_node->size);
        cached_nodes[index].size = p_node->size;
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
  inline tree_node *__attribute__((always_inline))getRoot(void){
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
  inline tree_node * __attribute__((always_inline))getPathFromRoot(uint8_t depth){
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
   * \param   index  The index of the to be set cached node in cached_nodes
   *
   * \return         Pinter to the tree path or NULL, if depth is greater then TREE_HEIGTH 
   */
  inline tree_node* __attribute__((always_inline))getPathFromRootThenCachedNodes(unsigned int depth, unsigned int index){
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
   * \param   index  The index of the to be set cached node in cached_nodes
   *
   * \return         Pinter to the tree path or NULL, if depth is greater then TREE_HEIGTH 
   */
  inline tree_node * __attribute__((always_inline))getPathFromCachedNodes(unsigned int depth, unsigned int index){
    if(depth > TREE_HEIGTH){
      return NULL;
    }
    int i = 1;
    if(index < CACHED_NODES_LEN && cached_nodes[index].size>0){
      memcpy(path_nodes+1, cached_nodes + index, sizeof(tree_node));
      i++;
    }
    for(; i-1<= depth; i++){
      path_nodes[i].size = 0;
    }
    return path_nodes+1;  
  }
  
  /*!
   * \brief Copy from tree node p_src to tree node p_dest 
   *
   * \param   p_dest Pointer to the destination tree node
   * \param   p_src  Pointer to the source tree node
   *
   * \return         Pinter to the destination tree node 
   */
  inline tree_node * __attribute__((always_inline))copyTreeNode(tree_node * p_dest, tree_node * p_src){
    if(p_src!=NULL && p_dest!=NULL){
      memcpy(p_dest->block, p_src->block, p_src->size);
      p_dest->size = p_src->size;
    }
    return p_dest;
  }

  /*!
   * \brief Compare two tree nodes 
   *
   * \param   p1 Pointer to the tree node 1
   * \param   p2 Pointer to the tree node 2
   *
   * \return  0, if tree node p1 is equal to tree node p2
   *          >0, if tree node p1 block is greater then p2 block
   *          <0, if tree node p1 block is smaller than p2 block
   */
  inline int __attribute__((always_inline))compareTreeNode(tree_node * p1, tree_node * p2){
    if(p1 == p2){
      return 0;
    }
    if(NULL==p1){
      return -1; 
    } 
    if(NULL==p2){
      return 1;
    }
    int size_diff = p1->size - p2->size;
    if(0!=size_diff){
      return size_diff;
    }
    return memcmp(p1->block, p2->block, p1->size);
  }
#endif

#ifdef  __cplusplus
}
#endif /* __cplusplus */

#endif /* __TREE_NODES_H__ */
/*!
 * \}
 */
