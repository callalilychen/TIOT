/*
 * treeconfig.h - Configuration to use the tree structure for generating secrets
 *
*/

#ifndef __TREE_CONFIG_H__
#define __TREE_CONFIG_H__

/*****************************************************************************/
/* Include files                                                             */
/*****************************************************************************/
#include "interface.h"

#ifdef  __cplusplus
extern "C" {
#endif
#define NODE_SIZE HASH_SIZE
#define HASH_FUNC sha_construction.func

#define MAX_LEVEL 2
#define MAX_CACHED_NODES 0

/*!
 * \brief Configuration of state manangement
 *
 *        States are managed in a table
 *        Each row in the table represents a state-vector
 *        A state-vector contains multiple states
 *
 * */
#define USE_STATE          /*!< Macro flag to indicate, whether a state table will be used for state management */
#define STATE_TABLE_LEN 0x4     /*!< Number of state vectors in the table (Number of table rows) */
#define STATE_VECTOR_LEN 2  /*!< Number of states in each state vector (Number of table columns) */

#define STATE_SIZE 2          /*!< Size of state in Bytes*/
#define STATE_TYPE uint16_t   /*!< Type of each state */
#define STATE_UPPER_BOUNDARY 0xff  /*!< All allowed states should be smaller than the STATE_UPPER_BOUNDARY */


/*!
 * \brief Configuration of bit map
 *
 *        The bitmap can be used with the state table together to indicate, whether a state vector can be updated 
 * */
#define USE_BIT_MAP                       /*!< Macro flag to indicate, whether a bit map will be used for state management */
#ifdef USE_STATE
#define BIT_MAP_LEN MAX_LEVEL-1                      /*!< Number of to used bit map*/
#if(STATE_TABLE_LEN & 0x7)
#define BIT_MAP_SIZE STATE_TABLE_LEN>>3   /*!< Size of bit map in bit */

#elif
#define BIT_MAP_SIZE (STATE_TABLE_LEN>>3) + 1
#endif
#endif


#ifdef  __cplusplus
}
#endif /* __cplusplus */

#endif /* __TREE_CONFIG_H__ */

