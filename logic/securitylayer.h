#ifndef __SECURITY_LAYER_H__
#define __SECURITY_LAYER_H__

#include "interface.h"
#include "hmac.h"
#include "treestate.h" 
#include "tree.h" 

#ifdef  __cplusplus
extern "C" {
#endif

//TODO value
#define NO_DESCRIPTOR 0xffff
#define MAX_VERSION 3
#define VERSION_BITS(x) (x>>6) & MAX_VERSION

  typedef struct securityLayerImplementation{
    unsigned char * (*parseHeader)(unsigned char* , unsigned int* , unsigned int *);

    void * (*getHeader)(unsigned int*);
    void * (*getPermCode)(unsigned int*);
    STATE_TYPE (*getSecretIndex)(void);
    STATE_TYPE (*getPermIndex)(void);
    STATE_TYPE (*getKeyIndex)(void);

    unsigned int (*setHeader)(unsigned char* , unsigned int);
    unsigned int (*setPermCode)(unsigned char* , unsigned int);
    unsigned int (*setSecretIndex)(unsigned char* , unsigned int);
    unsigned int (*setPermIndex)(unsigned char* , unsigned int);
    unsigned int (*setKeyIndex)(unsigned char* , unsigned int);

    unsigned int MACsize;
  }securityLayerImplementation;

  unsigned int createSecurityDescriptor(uint8_t, tree_node *);

  unsigned int handleSecurityLayer(unsigned char *, unsigned int*, unsigned int*);
  unsigned int generateSecurityLayerHeader(unsigned int, unsigned char*, unsigned int);
  unsigned int generateSecurityLayerMAC(unsigned int, unsigned char*, unsigned int, unsigned char*, unsigned int);

  inline void  (__attribute__((always_inline))removeSecurityLayerDescriptor)(unsigned int index){
    ;
  }


  void resetKey(void);


#ifdef  __cplusplus
}
#endif /* __cplusplus */

#endif /* __SECURITY_LAYER_H__ */
