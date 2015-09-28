#ifndef __ADDR_DESCRIPTOR_H__
#define __ADDR_DESCRIPTOR_H__

#include "treeconfig.h"
#include <string.h>

#ifdef  __cplusplus
extern "C" {
#endif

typedef struct addr_descriptor{
  unsigned char state;
  ADDR_TYPE addr;
} addr_descriptor;

extern addr_descriptor addr_descriptors[ADDR_DESCRIPTORS_LEN];
  
inline ADDR_TYPE * (__attribute__((always_inline))getAddr)(unsigned int index){
  if(index < ADDR_DESCRIPTORS_LEN){
    return &(addr_descriptors[index].addr);
  }
  return NULL;
}

inline void (__attribute__((always_inline))updateAddrDescriptor)(unsigned int index, unsigned int port, const char * ip){
  if(index < ADDR_DESCRIPTORS_LEN){
    addr_descriptors[index].addr.sin_family = ADDR_FAMILY;
    addr_descriptors[index].addr.sin_port = htons(port);
    ADDR_ATON(ip, &(addr_descriptors[index].addr.sin_addr.s_addr));
    addr_descriptors[index].state = DESCRIPTOR_ACTIVE;
  }
}

inline unsigned int (__attribute__((always_inline))addAddrDescriptors)(ADDR_TYPE * p_addr, size_t addrlen){
  unsigned int i=0;
  unsigned int first_inactive_index = NO_DESCRIPTOR;
  for(; i< ADDR_DESCRIPTORS_LEN;i++){
    if(addr_descriptors[i].state == DESCRIPTOR_ACTIVE && memcmp(p_addr, &(addr_descriptors[i].addr), addrlen)){
      return i;
    }else{
      if(NO_DESCRIPTOR == first_inactive_index){
        first_inactive_index = i;
      }
    }
  }
  if(i >= ADDR_DESCRIPTORS_LEN && first_inactive_index <= MAX_ADDR_DESCRIPTOR){
    memcpy(&(addr_descriptors[first_inactive_index].addr), p_addr,  addrlen);
    return first_inactive_index;
  }
  return NO_DESCRIPTOR;
}
#ifdef  __cplusplus
}
#endif /* __cplusplus */

#endif /* __ADDR_DESCRIPTOR_H__ */

