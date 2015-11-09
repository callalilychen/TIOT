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

#define SECURITY_LAYER_PROTOCOL_TYPE_BITS(x) (x & MAX_SECURITY_LAYER_PROTOCOL_TYPE) >>6 

  typedef struct securityLayerImplementation{
    unsigned int (*parseHeader)(unsigned int security_descriptor_id, unsigned char * msg, unsigned int *p_msg_size);
    unsigned int (*generateHeader)(unsigned int security_descriptor_id, unsigned char* msg, unsigned int msg_size);
    unsigned int (*generatePermCode)(unsigned int security_descriptor_id, unsigned char* msg, unsigned int msg_size);

    void * (*getHeader)(unsigned int security_descriptor_id, unsigned int *size);
    void * (*getPermCode)(unsigned int security_descriptor_id, unsigned int * size);
    TREE_STATE_TYPE (*getSecretIndex)(unsigned int security_descriptor_id);
    TREE_STATE_TYPE (*getPermIndex)(unsigned int security_descriptor_id);
    RIGHT_TYPE (*getPerm)(unsigned int security_descriptor_id);
    TREE_STATE_TYPE (*getKeyIndex)(unsigned int security_descriptor_id);

    unsigned int (*setSecretIndex)(unsigned int security_descriptor_id, TREE_STATE_TYPE index);
    unsigned int (*setPermIndex)(unsigned int security_descriptor_id, TREE_STATE_TYPE index);
    unsigned int (*setPerm)(unsigned int security_descriptor_id, RIGHT_TYPE index);
    unsigned int (*setKeyIndex)(unsigned int security_descriptor_id, TREE_STATE_TYPE index);
    unsigned int (*setPermCode)(unsigned int security_descriptor_id, unsigned char* msg, unsigned int msg_size);

    unsigned int HeaderSize;
    unsigned int MACsize;
  }securityLayerImplementation;

  extern const securityLayerImplementation* implementations[SECURITY_LAYER_IMPLEMENTATIONS_LEN];
  
  unsigned int handleSecurityLayer(unsigned char *, unsigned int*, unsigned int*);

  inline unsigned int __attribute__((always_inline))generateSecurityLayerHeader(unsigned int security_descriptor_id, unsigned char *buf, unsigned int max_buf_size){
    uint8_t currType = getDescriptorProtocolType(security_descriptor_id);
    if(currType>=SECURITY_LAYER_IMPLEMENTATIONS_LEN){
      return 0;
    }
    return implementations[currType]->generateHeader(security_descriptor_id, buf, max_buf_size);
  }

  inline unsigned int __attribute__((always_inline))generatePermCode(unsigned int security_descriptor_id, unsigned char *buf, unsigned int max_buf_size){
    uint8_t currType = getDescriptorProtocolType(security_descriptor_id);
    if(currType<SECURITY_LAYER_IMPLEMENTATIONS_LEN){
      return implementations[currType]->generatePermCode(security_descriptor_id, buf, max_buf_size);
    }
    return 0;
  }

  extern unsigned char tmpMAC[HASH_SIZE];

  inline unsigned int __attribute__((always_inline))generateSecurityLayerMAC(unsigned int security_descriptor_id, unsigned char *payload, unsigned int payload_size, unsigned int max_buf_size){
    uint8_t currType = getDescriptorProtocolType(security_descriptor_id);
    if(currType>=SECURITY_LAYER_IMPLEMENTATIONS_LEN || max_buf_size-payload_size < implementations[currType]->MACsize){
      return 0;
    }
    unsigned int mac_size = 0;
    tree_node * currKeyNode = getDescriptorKey(security_descriptor_id);
    if(NULL == currKeyNode){
      return 0;
    }
    //TEST_SIGNAL_LOW;

    hmac(&sha_construction, currKeyNode->block, currKeyNode->size, payload, payload_size, tmpMAC, &mac_size);
//  PRINT("=======Generate MAC %u========\n", security_descriptor_id);
//  printBlock("key", currKeyNode->block, currKeyNode->size);
//  printBlock("msg", payload, payload_size);
//  printBlock("Mac", tmpMAC, mac_size);
//  PRINT("============================\n");
    mac_size = implementations[currType]->MACsize;
    memcpy(payload+payload_size, tmpMAC, mac_size);
    //TEST_SIGNAL_HIGH;
    return mac_size;
  }

  inline unsigned int __attribute__((always_inline))setSecretIndex(unsigned int security_descriptor_id, TREE_STATE_TYPE index){
    uint8_t currType = getDescriptorProtocolType(security_descriptor_id);
    if(currType<SECURITY_LAYER_IMPLEMENTATIONS_LEN){
      return implementations[currType]->setSecretIndex(security_descriptor_id, index);
    }
    return 0;
  }  
     
  inline unsigned int __attribute__((always_inline))setPermIndex(unsigned int security_descriptor_id, TREE_STATE_TYPE index){
    uint8_t currType = getDescriptorProtocolType(security_descriptor_id);
    if(currType<SECURITY_LAYER_IMPLEMENTATIONS_LEN){
      return implementations[currType]->setPermIndex(security_descriptor_id, index);
    }
    return 0;
  }

  inline unsigned int __attribute__((always_inline))setPerm(unsigned int security_descriptor_id, RIGHT_TYPE right){
    uint8_t currType = getDescriptorProtocolType(security_descriptor_id);
    if(currType<SECURITY_LAYER_IMPLEMENTATIONS_LEN){
      return implementations[currType]->setPerm(security_descriptor_id, right);
    }
    return 0;
  }
  
  inline unsigned int __attribute__((always_inline))setKeyIndex(unsigned int security_descriptor_id, TREE_STATE_TYPE index){
    uint8_t currType = getDescriptorProtocolType(security_descriptor_id);
    if(currType<SECURITY_LAYER_IMPLEMENTATIONS_LEN){
      return implementations[currType]->setKeyIndex(security_descriptor_id, index);
    }
    return 0;
  }

  inline unsigned int __attribute__((always_inline))getSecurityLayerHeaderSize(unsigned int security_descriptor_id){
    uint8_t currType = getDescriptorProtocolType(security_descriptor_id);
    if(currType<SECURITY_LAYER_IMPLEMENTATIONS_LEN){
      return implementations[currType]->HeaderSize;
    }
    return 0;
    
  }

  inline unsigned int __attribute__((always_inline))getSecurityLayerMACSize(unsigned int security_descriptor_id){
    uint8_t currType = getDescriptorProtocolType(security_descriptor_id);
    if(currType<SECURITY_LAYER_IMPLEMENTATIONS_LEN){
      return implementations[currType]->MACsize;
    }
    return 0;
    
  }

  inline RIGHT_TYPE __attribute__((always_inline))getPerm(unsigned int security_descriptor_id){
    uint8_t currType = getDescriptorProtocolType(security_descriptor_id);
    if(currType<SECURITY_LAYER_IMPLEMENTATIONS_LEN){
      return implementations[currType]->getPerm(security_descriptor_id);
    }
    return NO_RIGHT;
  }

  inline TREE_STATE_TYPE __attribute__((always_inline))getSecretIndex(unsigned int security_descriptor_id){
    uint8_t currType = getDescriptorProtocolType(security_descriptor_id);
    if(currType<SECURITY_LAYER_IMPLEMENTATIONS_LEN){
      return implementations[currType]->getSecretIndex(security_descriptor_id);
    }
    return NO_TREE_STATE;
  }
  inline RIGHT_TYPE __attribute__((always_inline))getPermIndex(unsigned int security_descriptor_id){
    uint8_t currType = getDescriptorProtocolType(security_descriptor_id);
    if(currType<SECURITY_LAYER_IMPLEMENTATIONS_LEN){
      return implementations[currType]->getPermIndex(security_descriptor_id);
    }
    return NO_RIGHT;
  }


  inline TREE_STATE_TYPE __attribute__((always_inline))getKeyIndex(unsigned int security_descriptor_id){
    uint8_t currType = getDescriptorProtocolType(security_descriptor_id);
    if(currType<SECURITY_LAYER_IMPLEMENTATIONS_LEN){
      return implementations[currType]->getKeyIndex(security_descriptor_id);
    }
    return NO_TREE_STATE;
  }

  inline void *__attribute__((always_inline))getPermCode(unsigned int security_descriptor_id, unsigned int * size){
    uint8_t currType = getDescriptorProtocolType(security_descriptor_id);
    if(currType<SECURITY_LAYER_IMPLEMENTATIONS_LEN){
      return implementations[currType]->getPermCode(security_descriptor_id, size);
    }
    return NULL;
  }
  inline void __attribute__((always_inline))printSecurityDescriptorSecurityLayerHeader(){
    PRINT("Security Index\tPermission Index\tPermission\tKey Index");
  }
  
  inline void __attribute__((always_inline)) printSecurityDescriptorSecurityLayer(unsigned int id){
    PRINT("%u\t\t%u\t\t\t%u\t\t%u", 
        getSecretIndex(id),
        getPermIndex(id),
        getPerm(id),
        getKeyIndex(id));
  }

  inline int __attribute__((always_inline))copySecurityDescriptorSecurityLayer(unsigned int dest_id, unsigned int src_id){
    if(setSecretIndex(dest_id, getSecretIndex(src_id))+
    setPermIndex(dest_id, getPermIndex(src_id))+
    setPerm(dest_id, getPerm(src_id))+
    setKeyIndex(dest_id, getKeyIndex(src_id))>0){
      return SUCC;
    }
    return FAIL;
  }
#ifdef  __cplusplus
}
#endif /* __cplusplus */

#endif /* __SECURITY_LAYER_HANDLER_H__ */
/*!
 * \}
 */
