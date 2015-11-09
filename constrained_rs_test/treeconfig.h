/*
 * treeconfig.h - Configuration for constrained resource server
 *
*/
/*!
 * \defgroup    config_rs Configuration for an resource server
 *    
 * \brief All parties share the same data declaration, only the length of data are here to configure.
 *
 * \note         Perhaps too static!
 *
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
#define MSP
#ifdef MSP
#include "cli_uart.h"
#include "board.h"
#include "board_ext.h"
#include "simplelink.h"
#endif
#include "interface.h"
#include "printString.h"
#include "scanString.h"
#include "tmp.h"
#ifdef  __cplusplus
extern "C" {
#endif
    
  #define BUFSIZE 100

//TODO
//#define SECURITY_LAYER_UPDATE_STATE
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

#define IP_TYPE IPv4
#define IPv4 uint32_t 
#define HTONS sl_Htons
#define HTONL sl_Htonl
#define ASSIGN_IP(addr, ip) addr = HTONL(ip)

#define ADDR_FAMILY SL_AF_INET
#define ADDR_TYPE SlSockAddrIn_t
#define ADDR_SEND_TYPE SlSockAddr_t
#define ADDR_LEN_TYPE SlSocklen_t
#define SENDTO_FUNC(...) 

#define ADDR_DESCRIPTORS_LEN 1 
#define ADDR_PREDEF_LEN 1
#define PREDEF_AS_ADDR ADDR_DESCRIPTORS_LEN

#define SECURITY_DESCRIPTORS_LEN 1
#define SECURITY_PREDEF_LEN 0 
//#define PREDEF_NO_SECURITY_DESCRIPTOR SECURITY_DESCRIPTORS_LEN
#define SECURITY_LAYER_IMPLEMENTATIONS_LEN 1

#define NODE_SIZE HASH_SIZE
#define HASH_FUNC sha_construction.func

#define APPLICATION_SESSIONS_LEN 2
#define MAX_APPLICATION_MESSAGE_SIZE 50 

#define MSG_APPLICATION_COUNT 9
#define UI_APPLICATION_COUNT 0 
#define MAX_APPLICATION_NAME_SIZE 5
#define MAX_APPLICATION_USAGE_SIZE 0

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
#define TREE_STATE_TABLE_LEN 0xf     /*!< Number of state vectors in the table (Number of table rows) */
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

#define LED_IS_ON 1
#define LED_IS_OFF 0
#define RED_LED_ON turnLedOn(LED1)
#define RED_LED_OFF turnLedOff(LED1)
#define RED_LED_STATUS GetLEDStatus()&1

#define GREEN_LED_ON turnLedOn(LED2)
#define GREEN_LED_OFF turnLedOff(LED2)
#define GREEN_LED_STATUS (GetLEDStatus()>>1)

#define TEST_SIGNAL_HIGH  DEBUG("HIGH!\n");signalHigh()
#define TEST_SIGNAL_LOW DEBUG("LOW!\n");signalLow()


#ifdef  __cplusplus
}
#endif /* __cplusplus */

#endif /* __TREE_CONFIG_H__ */
/*!
 * \}
 */
