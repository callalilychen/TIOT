/*-
 * securitylayerexample.h - Security layer protocol example with type 0 
 *
 * Copyright 2015 Wenwen Chen
*/

/*!
 * \defgroup    securitylayerprotocol Security layer protocol implementation 
 * \{
 *
 * \file
 * \brief       Header definitions for the security layer protocol example with type 0
 *
 *              The protocol defines the following message format
 *
 * \verbatim
        0               1               2               3               Byte
        0 1 2 3 4 5 6 7 0 1 2 3 4 5 6 7 0 1 2 3 4 5 6 7 0 1 2 3 4 5 6 7 bit
        +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+   
        |Typ|PCT| TL  |C|                    Permission Code (Type 0)                
        |       0       |     PCT       | Security Index|   Perm Index   
        +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+     
           Permission   | Key Set index |                               /
        +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+                               \
        \                                                               /
        /                           Payload*                            \
        \                                                               /
        /                                                               \
        +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
        |                                                               |
        |                                                               |
        |                         MAC (20 Bytes)                        |
        |                                                               |
        |                                                               |
        +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
 \endverbatim
 * - Type: protocol type
 * - Permission code type (PCT): type of permission code 
 * - Token Length (TL): Length of token
 * - CipherFlag (C): Flag indicates, if the key usage field is attached. If not, default key usage shall be used. 
 * - Permission Code  (PC): 
 * - Token: TL Bytes
 * - **Key Set** index: the index of the used key set for this message. This field size is 1 byte.
 * - Payload: Plaintext, not encrypted
 * - MAC: use HMAC, which should be 20 Bytes. If SHA-256 is used, just cut the hash value and take the first 20 bytes. 
 *     - MAC is over transport layer payload, that means Transport layer security header + payload
 * - Totally, 5 Bytes header overhead + MAC (20 Bytes)
 * 
 * For this example implementation, the first byte of header is always 0 to achieve a minimal setting.
 *
 * \author      Wenwen Chen 
 */
#ifndef __SECURITY_LAYER_EXAMPLE_H__
#define __SECURITY_LAYER_EXAMPLE_H__

#include "treestate.h"
#ifdef  __cplusplus
extern "C" {
#endif

#define SECURITY_LAYER_TYPE_EXAMPLE 0
#define SECURITY_LAYER_HEADER_LEN_EXAMPLE 6
#define SECURITY_LAYER_PERMCODE_LEN_EXAMPLE 4
#define MAC_LEN_EXAMPLE 20
const uint8_t PERMCODE_TYPE;
#pragma pack(1)
  typedef struct permCodeExample{
    uint8_t permcode_type;
    uint8_t secret_index;
    uint8_t perm_index;
    uint8_t perm;/*!<Right informations*/
  }permCodeExample;

  typedef struct securityHeaderExample{
    uint8_t indicator;
    permCodeExample perm_code;
    uint8_t key_index;
  }securityHeaderExample;
#pragma pack()

  unsigned int parseHeaderExample(unsigned int security_descriptor, unsigned char * msg, unsigned int *p_msg_size);
  unsigned int generateHeaderExample(unsigned int security_descriptor, unsigned char* msg, unsigned int msg_size);
  unsigned int generatePermCodeExample(unsigned int security_descriptor, unsigned char* msg, unsigned int msg_size);

  void * getHeaderExample(unsigned int security_descriptor, unsigned int *size);
  void * getPermCodeExample(unsigned int security_descriptor, unsigned int * size);
  TREE_STATE_TYPE getSecretIndexExample(unsigned int security_descriptor);
  TREE_STATE_TYPE getPermIndexExample(unsigned int security_descriptor);
  RIGHT_TYPE getPermExample(unsigned int security_descriptor);
  TREE_STATE_TYPE getKeyIndexExample(unsigned int security_descriptor);

  unsigned int setSecretIndexExample(unsigned int security_descriptor, TREE_STATE_TYPE index);
  unsigned int setPermIndexExample(unsigned int security_descriptor, TREE_STATE_TYPE index);
  unsigned int setPermExample(unsigned int security_descriptor, RIGHT_TYPE index);
  unsigned int setKeyIndexExample(unsigned int security_descriptor, TREE_STATE_TYPE index);
  unsigned int setPermCodeExample(unsigned int security_descriptor, unsigned char* code, unsigned int code_size);

#ifdef  __cplusplus
}
#endif /* __cplusplus */

#endif /* __SECURITY_LAYER_EXAMPLE_H__ */
/*!
 * \}
 */

