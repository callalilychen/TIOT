#ifndef __SECURITY_LAYER_H__
#define __SECURITY_LAYER_H__

#include "interface.h"
#include "hmac.h"
#include "treestate.h" 
#include "tree.h" 
#include "protocolconfig.h"

#ifdef  __cplusplus
extern "C" {
#endif

#define MAX_SECURITY_LAYER_VERSION 3
#define SECURITY_LAYER_VERSION_BITS(x) (x>>6) & MAX_SECURITY_LAYER_VERSION

  typedef struct securityLayerImplementation{
    unsigned int (*parseHeader)(unsigned int security_descriptor, unsigned char * msg, unsigned int *p_msg_size);

    void * (*getHeader)(unsigned int security_descriptor, unsigned int *size);
    void * (*getPermCode)(unsigned int security_descriptor, unsigned int * size);
    STATE_TYPE (*getSecretIndex)(unsigned int security_descriptor);
    STATE_TYPE (*getPermIndex)(unsigned int security_descriptor);
    STATE_TYPE (*getKeyIndex)(unsigned int security_descriptor);

    unsigned int (*setHeader)(unsigned int security_descriptor, unsigned char* msg, unsigned int msg_size);
    unsigned int (*setSecretIndex)(unsigned int security_descriptor, unsigned char* msg, unsigned int msg_size);
    unsigned int (*setPermCode)(unsigned int security_descriptor, unsigned char* msg, unsigned int msg_size);
    unsigned int (*setPermIndex)(unsigned int security_descriptor, unsigned char* msg, unsigned int msg_size);
    unsigned int (*setKeyIndex)(unsigned int security_descriptor, unsigned char* msg, unsigned int msg_size);

    unsigned int MACsize;
  }securityLayerImplementation;

  unsigned int handleSecurityLayer(unsigned char *, unsigned int*, unsigned int*);
  unsigned int generateSecurityLayerHeader(unsigned int, unsigned char*, unsigned int);
  unsigned int generateSecurityLayerMAC(unsigned int, unsigned char*, unsigned int, unsigned char*, unsigned int);

  typedef struct security_descriptor{
    uint8_t state;
    uint8_t implementationVersion;
    tree_node * usedKeyNode;
// TODO sock addr?
  }security_descriptor;

  extern security_descriptor security_descriptors[MAX_SECURITY_DESCRIPTOR];
  
  inline void (__attribute__((always_inline))resetKey)(unsigned int index){
    security_descriptors[index].usedKeyNode = NULL;
  }

  inline unsigned int  (__attribute__((always_inline))createSecurityDescriptor)(void){
    for(int i=0; i<MAX_SECURITY_DESCRIPTOR; i++){
      if(DESCRIPTOR_INACTIVE == security_descriptors[i].state){
        return i;
      }
    }
    return NO_DESCRIPTOR;
  }

  inline unsigned int (__attribute__((always_inline))activeSecurityDescriptor)(unsigned int index){
    if(index == NO_DESCRIPTOR || index >=MAX_SECURITY_DESCRIPTOR){
      return NO_DESCRIPTOR;
    }
    security_descriptors[index].state = DESCRIPTOR_ACTIVE;
    return index;
  }

  inline unsigned int (__attribute__((always_inline))updateSecurityDescriptor)(unsigned int index, uint8_t version, tree_node * p_key){
    if(index == NO_DESCRIPTOR || index > MAX_SECURITY_DESCRIPTOR){
      return NO_DESCRIPTOR;
    }

    security_descriptors[index].implementationVersion = version;
    security_descriptors[index].usedKeyNode = p_key;
    return index;
  }

  inline unsigned int (__attribute__((always_inline))addSecurityDescriptor)(uint8_t version, tree_node * p_key){
    unsigned int index = createSecurityDescriptor();
    if(index == updateSecurityDescriptor(index, version, p_key)){
        return activeSecurityDescriptor(index);
    }
    return NO_DESCRIPTOR;
  }

  inline void (__attribute__((always_inline))removeSecurityLayerDescriptor)(unsigned int index){
    if(index == NO_DESCRIPTOR || index >=MAX_SECURITY_DESCRIPTOR){
      return;
    }
    security_descriptors[index].state = DESCRIPTOR_INACTIVE;
  }


#ifdef  __cplusplus
}
#endif /* __cplusplus */

#endif /* __SECURITY_LAYER_H__ */
