/*_
 * treeconfig.h - Configuration for an resource server
 *
 * Copyright 2015 Wenwen Chen
 */
/*!
 * \defgroup    config_rs Configuration for an resource server
 * \{
 *
 * \file
 * \brief       Configuration for an resource server
 *
 * \author      Wenwen Chen 
 */
#ifndef __TREE_CONFIG_H__
#define __TREE_CONFIG_H__

/*****************************************************************************/
/* Include files                                                             */
/*****************************************************************************/
#include "interface.h"
#include <sys/socket.h>
#include <netinet/in.h>

#include "printString.h"
#include "scanString.h"

#ifdef  __cplusplus
extern "C" {
#endif
/*!
 * \brief Macro for print function
 */
#define PRINT(...) printf(__VA_ARGS__)
#define SCAN(...) scanf(__VA_ARGS__)
#define SSCAN(...) sscan(__VA_ARGS__)
#define SPRINT(...) sprint(__VA_ARGS__)

/*!
 * \brief Macro to indicate, whether an operation is successful
 */
#define SUCC 0
/*!
 * \brief Macro to indicate, whether an operation failed
 */
#define FAIL -1

#define RIGHT_TYPE uint8_t
/*!
 * \brief Macro for when no right is required 
 */
#define NO_RIGHT 0x0
#define ADMIN_RIGHT 0xff

#define DEFAULT_PROTOCOL_TYPE 0

#define IP_TYPE IPv4
#define IPv4 char* 
#define HTONS htons
#define HTONL htonl
#define ASSIGN_IP(addr, ip) inet_aton(ip, &addr)

#define HTONS htons
#define ADDR_FAMILY AF_INET
#define ADDR_TYPE struct sockaddr_in
#define ADDR_SEND_TYPE struct sockaddr
#define ADDR_LEN_TYPE socklen_t
#define SENDTO_FUNC sendto
  
#define ADDR_DESCRIPTORS_LEN 3 
#define ADDR_PREDEF_LEN 1
#define PREDEF_AS_ADDR ADDR_DESCRIPTORS_LEN

#define SECURITY_DESCRIPTORS_LEN 3
#define SECURITY_PREDEF_LEN 1 
#define PREDEF_NO_SECURITY_DESCRIPTOR SECURITY_DESCRIPTORS_LEN

#define NODE_SIZE HASH_SIZE
#define HASH_FUNC sha_construction.func

#define APPLICATION_SESSIONS_LEN 2
#define MAX_APPLICATION_MESSAGE_SIZE 200  

#define MSG_APPLICATION_COUNT 4
#define UI_APPLICATION_COUNT 0 
#define MAX_APPLICATION_NAME_SIZE 5

/*!
 * \brief Configuration of tree
 *
 *        0. level -> root
 *        1. level -> secrets
 *        2. level -> key 
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
#define TREE_STATE_TABLE_LEN 0xff     /*!< Number of state vectors in the table (Number of table rows) */
#define TREE_STATE_VECTOR_LEN 2  /*!< Number of states in each state vector (Number of table columns) */

#define TREE_STATE_SIZE 2          /*!< Size of state in Bytes*/
#define TREE_STATE_TYPE uint16_t   /*!< Type of each state */
#define TREE_STATE_UPPER_BOUNDARY 0xffff  /*!< All allowed states should be smaller than the STATE_UPPER_BOUNDARY */


/*!
 * \brief Configuration of bit map
 *
 *        The bitmap can be used with the state table together to indicate, whether a state can be updated 
 * */
//#define USE_BIT_MAP                       /*!< Macro flag to indicate, whether a bit map will be used for state management */
//#define BIT_MAP_LEN TREE_STATE_TABLE_LEN                      /*!< Number of to used bit map*/


#ifdef  __cplusplus
}
#endif /* __cplusplus */

#endif /* __TREE_CONFIG_H__ */
/*!
 * \}
 */
