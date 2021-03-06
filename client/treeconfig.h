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
#include <sys/socket.h>
#include <netinet/in.h>

#ifdef  __cplusplus
extern "C" {
#endif
#define PRINT(...) printf(__VA_ARGS__)
#define SCAN(...) scanf(__VA_ARGS__)
#define SSCAN(...) sscanf(__VA_ARGS__)
#define SPRINT(...) sprintf(__VA_ARGS__)

#define RIGHT_TYPE uint32_t

#define DEFAULT_VERSION 0
#define MAX_SECURITY_LAYER_VERSION 0xff

#define NO_DESCRIPTOR 0xffff
#define DESCRIPTOR_ACTIVE 1
#define DESCRIPTOR_INACTIVE 0

#define SECURITY_DESCRIPTORS_LEN 3
#define MAX_SECURITY_DESCRIPTOR SECURITY_DESCRIPTORS_LEN - 1 
#define PREDEF_NO_SECURITY_DESCRIPTOR SECURITY_DESCRIPTORS_LEN-1

#define IP_TYPE IPv4
#define IPv4 char* 
#define HTONS htons
#define HTONL htonl
#define ASSIGN_IP(addr, ip) inet_aton(ip, &addr)

#define ADDR_FAMILY AF_INET
#define ADDR_TYPE struct sockaddr_in
#define ADDR_SEND_TYPE struct sockaddr
#define ADDR_LEN_TYPE socklen_t
#define SENDTO_FUNC sendto
#define ADDR_DESCRIPTORS_LEN 4 
#define MAX_ADDR_DESCRIPTOR ADDR_DESCRIPTORS_LEN - 3 
#define PREDEF_AS_ADDR ADDR_DESCRIPTORS_LEN-2
#define PREDEF_RS_ADDR ADDR_DESCRIPTORS_LEN-1

#define NODE_SIZE HASH_SIZE
#define HASH_FUNC sha_construction.func

#define APPLICATION_SESSIONS_LEN 2
#define MAX_APPLICATION_MESSAGE_SIZE 20

#define MSG_APPLICATION_COUNT 3
#define UI_APPLICATION_COUNT 5
#define MAX_APPLICATION_NAME_SIZE 5

/*!
 * \brief Configuration of tree
 *
 * */
#define TREE_HEIGTH 1
#define CACHED_NODES_LEN ADDR_DESCRIPTORS_LEN

/*!
 * \brief Configuration of state manangement
 *
 *        States are managed in a table
 *        Each row in the table represents a state-vector
 *        A state-vector contains multiple states
 *
 * */
#define USE_STATE          /*!< Macro flag to indicate, whether a state table will be used for state management */
#define STATE_TABLE_LEN 0xff     /*!< Number of state vectors in the table (Number of table rows) */
#define STATE_VECTOR_LEN 2  /*!< Number of states in each state vector (Number of table columns) */

#define STATE_SIZE 2          /*!< Size of state in Bytes*/
#define STATE_TYPE uint16_t   /*!< Type of each state */
#define STATE_UPPER_BOUNDARY 0xffff  /*!< All allowed states should be smaller than the STATE_UPPER_BOUNDARY */


/*!
 * \brief Configuration of bit map
 *
 *        The bitmap can be used with the state table together to indicate, whether a state can be updated 
 * */
#define USE_BIT_MAP                       /*!< Macro flag to indicate, whether a bit map will be used for state management */
#ifdef USE_STATE
#define BIT_MAP_LEN STATE_TABLE_LEN                      /*!< Number of to used bit map*/
#endif


#ifdef  __cplusplus
}
#endif /* __cplusplus */

#endif /* __TREE_CONFIG_H__ */

