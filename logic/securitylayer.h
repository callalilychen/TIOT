#ifndef __SECURITY_LAYER_H__
#define __SECURITY_LAYER_H__

#include "interface.h"
#include "securitydescriptor.h"
#include "hmac.h"
#include "treestate.h" 
#include "tree.h" 
#include "treeconfig.h"

#ifdef  __cplusplus
extern "C" {
#endif

#define SECURITY_LAYER_IMPLEMENTATIONS_LEN 1
#define SECURITY_LAYER_VERSION_BITS(x) (x & MAX_SECURITY_LAYER_VERSION) >>6 

  typedef struct securityLayerImplementation{
    unsigned int (*parseHeader)(unsigned int security_descriptor, unsigned char * msg, unsigned int *p_msg_size);
    unsigned int (*generateHeader)(unsigned int security_descriptor, unsigned char* msg, unsigned int msg_size);
    unsigned int (*generatePermCode)(unsigned int security_descriptor, unsigned char* msg, unsigned int msg_size);

    void * (*getHeader)(unsigned int security_descriptor, unsigned int *size);
    void * (*getPermCode)(unsigned int security_descriptor, unsigned int * size);
    STATE_TYPE (*getSecretIndex)(unsigned int security_descriptor);
    STATE_TYPE (*getPermIndex)(unsigned int security_descriptor);
    STATE_TYPE (*getKeyIndex)(unsigned int security_descriptor);

    void (*setSecretIndex)(unsigned int security_descriptor, STATE_TYPE index);
    void (*setPermIndex)(unsigned int security_descriptor, STATE_TYPE index);
    void (*setKeyIndex)(unsigned int security_descriptor, STATE_TYPE index);
    unsigned int (*setPermCode)(unsigned int security_descriptor, unsigned char* msg, unsigned int msg_size);

    unsigned int MACsize;
  }securityLayerImplementation;

  extern const securityLayerImplementation* implementations[SECURITY_LAYER_IMPLEMENTATIONS_LEN];
  
  unsigned int handleSecurityLayer(unsigned char *, unsigned int*, unsigned int*);

  inline void (__attribute__((always_inline))setKeyIndex)(unsigned int security_descriptor, STATE_TYPE index){
    if(security_descriptor > SECURITY_DESCRIPTORS_LEN){
      return;
    }
    uint8_t currVersion = security_descriptors[security_descriptor].implementationVersion;
    if(currVersion>=SECURITY_LAYER_IMPLEMENTATIONS_LEN){
      return;
    }
    implementations[currVersion]->setKeyIndex(security_descriptor, index);
    
  }

  inline unsigned int (__attribute__((always_inline))generateSecurityLayerHeader)(unsigned int security_descriptor, unsigned char *buf, unsigned int max_buf_size){
    if(security_descriptor > SECURITY_DESCRIPTORS_LEN){
      return 0;
    }
    uint8_t currVersion = security_descriptors[security_descriptor].implementationVersion;
    if(currVersion>=SECURITY_LAYER_IMPLEMENTATIONS_LEN){
      *buf = currVersion;
      return 1;
    }
    return implementations[currVersion]->generateHeader(security_descriptor, buf, max_buf_size);
  }

  inline unsigned int (__attribute__((always_inline))generatePermCode)(unsigned int security_descriptor, unsigned char *buf, unsigned int max_buf_size){
    if(security_descriptor > SECURITY_DESCRIPTORS_LEN){
      return 0;
    }
    uint8_t currVersion = security_descriptors[security_descriptor].implementationVersion;
    if(currVersion>=SECURITY_LAYER_IMPLEMENTATIONS_LEN){
      return implementations[DEFAULT_VERSION]->generatePermCode(security_descriptor, buf, max_buf_size);
    }
    return implementations[currVersion]->generatePermCode(security_descriptor, buf, max_buf_size);
  }

  extern unsigned char tmpMAC[HASH_SIZE];

  inline unsigned int (__attribute__((always_inline))generateSecurityLayerMAC)(unsigned int security_descriptor, unsigned char *payload, unsigned int payload_size, unsigned int max_buf_size){
    if(security_descriptor > MAX_SECURITY_DESCRIPTOR){
      return 0;
    }
    uint8_t currVersion = security_descriptors[security_descriptor].implementationVersion;
    if(currVersion>=SECURITY_LAYER_IMPLEMENTATIONS_LEN || max_buf_size-payload_size < implementations[currVersion]->MACsize){
      return 0;
    }
    unsigned int mac_size = 0;
    tree_node * currKeyNode = security_descriptors[security_descriptor].usedKeyNode;
    hmac(&sha_construction, currKeyNode->block, currKeyNode->size, payload, payload_size, tmpMAC, &mac_size);
    mac_size = implementations[currVersion]->MACsize;
    memcpy(payload+payload_size, tmpMAC, mac_size);
    return mac_size;
  }

  inline void (__attribute__((always_inline))setSecretIndex)(unsigned int security_descriptor, STATE_TYPE index){
    if(security_descriptor > MAX_SECURITY_DESCRIPTOR){
      return;
    }
    uint8_t currVersion = security_descriptors[security_descriptor].implementationVersion;
    if(currVersion>=SECURITY_LAYER_IMPLEMENTATIONS_LEN){
      currVersion = DEFAULT_VERSION;
    }
   implementations[currVersion]->setSecretIndex(security_descriptor, index);
  }

  inline void (__attribute__((always_inline))setPermIndex)(unsigned int security_descriptor, STATE_TYPE index){
    if(security_descriptor > MAX_SECURITY_DESCRIPTOR){
      return;
    }
    uint8_t currVersion = security_descriptors[security_descriptor].implementationVersion;
    if(currVersion>=SECURITY_LAYER_IMPLEMENTATIONS_LEN){
      currVersion = DEFAULT_VERSION;
    }
   implementations[currVersion]->setPermIndex(security_descriptor, index);
  }

  inline void *(__attribute__((always_inline))getPermCode)(unsigned int security_descriptor, unsigned int * size){
    if(security_descriptor > MAX_SECURITY_DESCRIPTOR){
      return NULL;
    }
    uint8_t currVersion = security_descriptors[security_descriptor].implementationVersion;
    if(currVersion>=SECURITY_LAYER_IMPLEMENTATIONS_LEN){
      currVersion = DEFAULT_VERSION;
    }
    return implementations[currVersion]->getPermCode(security_descriptor, size);
  }
#ifdef  __cplusplus
}
#endif /* __cplusplus */

#endif /* __SECURITY_LAYER_H__ */
