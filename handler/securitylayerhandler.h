/*-
 * securitylayerhandler.h - Security layer handler 
 *
 * Copyright 2015 Wenwen Chen
*/

/*!
 * \addtogroup    handler
 * \{
 *
 * \file
 * \brief       Implementation of the security layer functions
 *
 * \author      Wenwen Chen 
 */
#ifndef __SECURITY_LAYER_HANDLER_H__
#define __SECURITY_LAYER_HANDLER_H__

#include "treeconfig.h"
#include "utils.h"
#include "securitydescriptor.h"
#include "hmac.h"
#include "tree.h" 
#include "treestate.h" 

#ifdef  __cplusplus
extern "C" {
#endif
#define MAX_SECURITY_LAYER_PROTOCOL_TYPE 0x3
#define SECURITY_LAYER_IMPLEMENTATIONS_LEN 1
#define SECURITY_LAYER_PROTOCOL_TYPE_BITS(x) (x & MAX_SECURITY_LAYER_PROTOCOL_TYPE) >>6 

  typedef struct securityLayerImplementation{
    unsigned int (*parseHeader)(unsigned int security_descriptor, unsigned char * msg, unsigned int *p_msg_size);
    unsigned int (*generateHeader)(unsigned int security_descriptor, unsigned char* msg, unsigned int msg_size);
    unsigned int (*generatePermCode)(unsigned int security_descriptor, unsigned char* msg, unsigned int msg_size);

    void * (*getHeader)(unsigned int security_descriptor, unsigned int *size);
    void * (*getPermCode)(unsigned int security_descriptor, unsigned int * size);
    TREE_STATE_TYPE (*getSecretIndex)(unsigned int security_descriptor);
    TREE_STATE_TYPE (*getPermIndex)(unsigned int security_descriptor);
    RIGHT_TYPE (*getPerm)(unsigned int security_descriptor);
    TREE_STATE_TYPE (*getKeyIndex)(unsigned int security_descriptor);

    void (*setSecretIndex)(unsigned int security_descriptor, TREE_STATE_TYPE index);
    void (*setPermIndex)(unsigned int security_descriptor, TREE_STATE_TYPE index);
    void (*setPerm)(unsigned int security_descriptor, RIGHT_TYPE index);
    void (*setKeyIndex)(unsigned int security_descriptor, TREE_STATE_TYPE index);
    unsigned int (*setPermCode)(unsigned int security_descriptor, unsigned char* msg, unsigned int msg_size);

    unsigned int MACsize;
  }securityLayerImplementation;

  extern const securityLayerImplementation* implementations[SECURITY_LAYER_IMPLEMENTATIONS_LEN];
  
  unsigned int handleSecurityLayer(unsigned char *, unsigned int*, unsigned int*);

  inline void __attribute__((always_inline))setKeyIndex(unsigned int security_descriptor, TREE_STATE_TYPE index){
    uint8_t currType = getDescriptorProtocolType(security_descriptor);
    if(currType>=SECURITY_LAYER_IMPLEMENTATIONS_LEN){
      return;
    }
    implementations[currType]->setKeyIndex(security_descriptor, index);
    
  }

  inline unsigned int __attribute__((always_inline))generateSecurityLayerHeader(unsigned int security_descriptor, unsigned char *buf, unsigned int max_buf_size){
    uint8_t currType = getDescriptorProtocolType(security_descriptor);
    if(currType>=SECURITY_LAYER_IMPLEMENTATIONS_LEN){
      *buf = currType;
      return 1;
    }
    return implementations[currType]->generateHeader(security_descriptor, buf, max_buf_size);
  }

  inline unsigned int __attribute__((always_inline))generatePermCode(unsigned int security_descriptor, unsigned char *buf, unsigned int max_buf_size){
    uint8_t currType = getDescriptorProtocolType(security_descriptor);
    if(currType>=SECURITY_LAYER_IMPLEMENTATIONS_LEN){
      return implementations[DEFAULT_PROTOCOL_TYPE]->generatePermCode(security_descriptor, buf, max_buf_size);
    }
    return implementations[currType]->generatePermCode(security_descriptor, buf, max_buf_size);
  }

  extern unsigned char tmpMAC[HASH_SIZE];

  inline unsigned int __attribute__((always_inline))generateSecurityLayerMAC(unsigned int security_descriptor, unsigned char *payload, unsigned int payload_size, unsigned int max_buf_size){
    uint8_t currType = getDescriptorProtocolType(security_descriptor);
    if(currType>=SECURITY_LAYER_IMPLEMENTATIONS_LEN || max_buf_size-payload_size < implementations[currType]->MACsize){
      return 0;
    }
    unsigned int mac_size = 0;
    tree_node * currKeyNode = getDescriptorKey(security_descriptor);
    if(NULL == currKeyNode){
      return 0;
    }
    hmac(&sha_construction, currKeyNode->block, currKeyNode->size, payload, payload_size, tmpMAC, &mac_size);
    mac_size = implementations[currType]->MACsize;
    memcpy(payload+payload_size, tmpMAC, mac_size);
    return mac_size;
  }

  inline void __attribute__((always_inline))setSecretIndex(unsigned int security_descriptor, TREE_STATE_TYPE index){
    uint8_t currType = getDescriptorProtocolType(security_descriptor);
    if(currType>=SECURITY_LAYER_IMPLEMENTATIONS_LEN){
      currType = DEFAULT_PROTOCOL_TYPE;
    }
   implementations[currType]->setSecretIndex(security_descriptor, index);
  }

  inline void __attribute__((always_inline))setPermIndex(unsigned int security_descriptor, TREE_STATE_TYPE index){
    uint8_t currType = getDescriptorProtocolType(security_descriptor);
    if(currType>=SECURITY_LAYER_IMPLEMENTATIONS_LEN){
      currType = DEFAULT_PROTOCOL_TYPE;
    }
   implementations[currType]->setPermIndex(security_descriptor, index);
  }

  inline void __attribute__((always_inline))setPerm(unsigned int security_descriptor, RIGHT_TYPE right){
    uint8_t currType = getDescriptorProtocolType(security_descriptor);
    if(currType>=SECURITY_LAYER_IMPLEMENTATIONS_LEN){
      currType = DEFAULT_PROTOCOL_TYPE;
    }
   implementations[currType]->setPerm(security_descriptor, right);
  }

  inline void *__attribute__((always_inline))getPermCode(unsigned int security_descriptor, unsigned int * size){
    uint8_t currType = getDescriptorProtocolType(security_descriptor);
    if(currType>=SECURITY_LAYER_IMPLEMENTATIONS_LEN){
      currType = DEFAULT_PROTOCOL_TYPE;
    }
    return implementations[currType]->getPermCode(security_descriptor, size);
  }
#ifdef  __cplusplus
}
#endif /* __cplusplus */

#endif /* __SECURITY_LAYER_HANDLER_H__ */
/*!
 * \}
 */
