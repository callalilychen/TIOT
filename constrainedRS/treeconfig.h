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
#include "simplelink.h"

#ifdef  __cplusplus
extern "C" {
#endif
#define PRINT(...) print(__VA_ARGS__)
#define SCAN(...)  
#define SSCAN(...) sscan(__VA_ARGS__)  
#define SPRINT(...) 

#define SUCC 0
#define FAIL -1

#define RIGHT_TYPE uint32_t

#define DEFAULT_PROTOCOL_TYPE 0

#define IP_TYPE IPv4
#define IPv4 uint32_t 
#define HTONS sl_Htons
#define HTONL sl_Htonl
#define ASSIGN_IP(addr, ip) addr = HTONL(ip)

#define ADDR_FAMILY SL_AF_INET
#define ADDR_TYPE SlSockAddrIn_t
#define ADDR_SEND_TYPE SlSockAddr_t
#define ADDR_LEN_TYPE SlSocklen_t
#define SENDTO_FUNC sl_SendTo

#define ADDR_DESCRIPTORS_LEN 3 
#define ADDR_PREDEF_LEN 1
#define PREDEF_AS_ADDR ADDR_DESCRIPTORS_LEN

#define SECURITY_DESCRIPTORS_LEN 3
#define SECURITY_PREDEF_LEN 1 
#define PREDEF_NO_SECURITY_DESCRIPTOR SECURITY_DESCRIPTORS_LEN

#define NODE_SIZE HASH_SIZE
#define HASH_FUNC sha_construction.func

#define APPLICATION_SESSIONS_LEN 2
#define MAX_APPLICATION_MESSAGE_SIZE 20 

#define MSG_APPLICATION_COUNT 4
#define UI_APPLICATION_COUNT 0 
#define MAX_APPLICATION_NAME_SIZE 5

/*!
 * \brief Configuration of tree
 *
 *        0. level -> root
 *        1. level -> secrets
 *        2. level -> key // FIXME AS can also behavior as C
 *
 * */
#define TREE_HEIGTH 2
#define CACHED_NODES_LEN ADDR_DESCRIPTORS_LEN

/*!
 * \brief Configuration of state manangement
 *
 *        States are managed in a table
 *        Each row in the table represents a state-vector
 *        A state-vector contains multiple states
 *
 * */
#define USE_TREE_STATE          /*!< Macro flag to indicate, whether a state table will be used for state management */
#define TREE_STATE_TABLE_LEN 0xff     /*!< Number of state vectors in the table (Number of table rows) */
#define TREE_STATE_VECTOR_LEN 2  /*!< Number of states in each state vector (Number of table columns) */

#define TREE_STATE_SIZE 2          /*!< Size of state in Bytes*/
#define TREE_STATE_TYPE uint16_t   /*!< Type of each state */
#define TREE_STATE_UPPER_BOUNDARY 0xffff  /*!< All allowed states should be smaller than the TREE_STATE_UPPER_BOUNDARY */


/*!
 * \brief Configuration of bit map
 *
 *        The bitmap can be used with the state table together to indicate, whether a state can be updated 
 * */
//#define USE_BIT_MAP                       /*!< Macro flag to indicate, whether a bit map will be used for state management */
//#ifdef USE_TREE_STATE
//#define BIT_MAP_LEN TREE_STATE_TABLE_LEN                      /*!< Number of to used bit map*/
//#endif


#ifdef  __cplusplus
}
#endif /* __cplusplus */

#endif /* __TREE_CONFIG_H__ */

