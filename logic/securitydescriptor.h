#ifndef __SECURITY_DESCRIPTOR_H__
#define __SECURITY_DESCRIPTOR_H__

#include "tree.h" 
#include "treeconfig.h"

#ifdef  __cplusplus
extern "C" {
#endif


  typedef struct security_descriptor{
    uint8_t state;
    uint8_t implementationVersion;
    tree_node * usedKeyNode;
  }security_descriptor;

  extern security_descriptor security_descriptors[SECURITY_DESCRIPTORS_LEN];
  
  inline void (__attribute__((always_inline))resetKey)(unsigned int index){
    if(index < SECURITY_DESCRIPTORS_LEN){
      security_descriptors[index].usedKeyNode = NULL;
    }
  }
  
  inline void(__attribute__((always_inline))initSecurityDescriptor)(void){
    security_descriptors[PREDEF_NO_SECURITY_DESCRIPTOR].state = DESCRIPTOR_ACTIVE;
    security_descriptors[PREDEF_NO_SECURITY_DESCRIPTOR].implementationVersion = MAX_SECURITY_LAYER_VERSION;
    security_descriptors[PREDEF_NO_SECURITY_DESCRIPTOR].usedKeyNode = NULL;
  }

  inline unsigned int  (__attribute__((always_inline))createSecurityDescriptor)(void){
    for(int i=0; i<=MAX_SECURITY_DESCRIPTOR; i++){
      if(DESCRIPTOR_INACTIVE == security_descriptors[i].state){
        return i;
      }
    }
    return NO_DESCRIPTOR;
  }

  inline unsigned int (__attribute__((always_inline))activeSecurityDescriptor)(unsigned int index){
    if(index >MAX_SECURITY_DESCRIPTOR){
      return NO_DESCRIPTOR;
    }
    security_descriptors[index].state = DESCRIPTOR_ACTIVE;
    return index;
  }

  inline unsigned int (__attribute__((always_inline))updateKey)(unsigned int index, tree_node * p_key){
    if(index >= SECURITY_DESCRIPTORS_LEN){
      return NO_DESCRIPTOR;
    }

    security_descriptors[index].usedKeyNode = p_key;
    return index;
  }

  inline unsigned int (__attribute__((always_inline))updateImplementationVersion)(unsigned int index, uint8_t version){
    if(index >= SECURITY_DESCRIPTORS_LEN){
      return NO_DESCRIPTOR;
    }

    security_descriptors[index].implementationVersion = version;
    return index;
  }

  inline unsigned int (__attribute__((always_inline))updateSecurityDescriptor)(unsigned int index, uint8_t version, tree_node * p_key){
    if(index >= SECURITY_DESCRIPTORS_LEN){
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
    if(index > MAX_SECURITY_DESCRIPTOR){
      return;
    }
    security_descriptors[index].state = DESCRIPTOR_INACTIVE;
  }




#ifdef  __cplusplus
}
#endif /* __cplusplus */

#endif /* __SECURITY_DESCRIPTOR_H__ */
