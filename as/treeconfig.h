/*_
 * treeconfig.h - Configuration for an authorization server
 *
 * Copyright 2015 Wenwen Chen
 */
/*!
 * \defgroup    config_as Configuration for an authorization server
 * \brief    All parties share the same data declaration, only the length of data are here to configure.
 *
 * \note         Perhaps too static!
 *
 * \{
 *
 * \file
 * \brief        Configuration for an authorization server
 *
 * \author      Wenwen Chen 
 */
#ifndef __TREE_CONFIG_H__
#define __TREE_CONFIG_H__

/*****************************************************************************/
/* Include files                                                             */
/*****************************************************************************/
#include "interface.h"
#include "scanString.h"
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#ifdef  __cplusplus
extern "C" {
#endif
#define BUFSIZE 512
/*!
 * \brief Macro for print function
 */
#define PRINT(...) printf(__VA_ARGS__)
#define SCAN(...) scanf(__VA_ARGS__)
#define SSCAN(...) sscanf(__VA_ARGS__)
#define SPRINT(...) sprintf(__VA_ARGS__)

/*!
 * \brief Macro to indicate, whether an operation is successful 
 */
#define SUCC 0
/*!
 * \brief Macro to indicate, whether an operation failed
 */
#define FAIL -1
/*!
 * \brief Macro to indicate, whether an operation is done
 */
#define DONE 1
/*!
 * \brief Macro to indicate infomations 
 */
#define INFO 2


#define SUCC_MESSAGE "[SUCCESS]"
#define SUCC_MESSAGE_SIZE 9

#define DONE_MESSAGE "[DONE]"
#define DONE_MESSAGE_SIZE 6

#define ERROR_MESSAGE "[ERROR]"
#define ERROR_MESSAGE_SIZE 7

#define USAGE_MESSAGE "[USAGE]"
#define USAGE_MESSAGE_SIZE 7

#define INFO_MESSAGE "[INFO]"
#define INFO_MESSAGE_SIZE 6


#define RIGHT_TYPE uint8_t
/*!
 * \brief Macro for when no right is required 
 */
#define NO_RIGHT 0x0
#define ADMIN_RIGHT 0xff
#define ADMIN_PASSWORD_HASH "t"

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
#define ADDR_PREDEF_LEN 1
#define PREDEF_RS_ADDR ADDR_DESCRIPTORS_LEN

#define SECURITY_DESCRIPTORS_LEN 3
#define SECURITY_PREDEF_LEN 1
#define PREDEF_TEST_SECURITY_DESCRIPTOR SECURITY_DESCRIPTORS_LEN
#define SECURITY_LAYER_IMPLEMENTATIONS_LEN 1

#define NODE_SIZE HASH_SIZE
#define HASH_FUNC sha_construction.func

#define APPLICATION_SESSIONS_LEN 2
#define MAX_APPLICATION_MESSAGE_SIZE 200  

#define RS_MSG_APPLICATION_COUNT 9
#define MSG_APPLICATION_COUNT 5
#define UI_APPLICATION_COUNT 23
#define MAX_APPLICATION_NAME_SIZE 20
#define MAX_APPLICATION_USAGE_SIZE 200

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
#define TREE_STATE_TABLE_LEN 0xff     /*!< Number of state vectors in the table (Number of table rows) */
#define TREE_STATE_VECTOR_LEN 2  /*!< Number of states in each state vector (Number of table columns) */

#define TREE_STATE_SIZE 2          /*!< Size of state in Bytes*/
#define TREE_STATE_TYPE uint16_t   /*!< Type of each state */
#define TREE_STATE_UPPER_BOUNDARY 0xff  /*!< All allowed states should be smaller than the TREE_STATE_UPPER_BOUNDARY */


/*!
 * \brief Configuration of bit map
 *
 *        The bitmap can be used with the state table together to indicate, whether a state can be updated 
 * */
#define USE_BIT_MAP                       /*!< Macro flag to indicate, whether a bit map will be used for state management */
#define BIT_MAP_LEN TREE_STATE_TABLE_LEN                      /*!< Number of to used bit map*/

#define LED_IS_ON 1
#define LED_IS_OFF 0
#define RED_LED_ON printf("RED LED is on!")
#define RED_LED_OFF printf("RED LED if off!")
#define RED_LED_STATUS LED_IS_ON

#define GREEN_LED_ON printf("GREEN LED is on!\n")
#define GREEN_LED_OFF printf("GREEN LED if off!\n")
#define GREEN_LED_STATUS LED_IS_ON



#ifdef  __cplusplus
}
#endif /* __cplusplus */

#endif /* __TREE_CONFIG_H__ */

