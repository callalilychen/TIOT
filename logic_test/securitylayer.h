#ifndef __SECURITY_LAYER_H__
#define __SECURITY_LAYER_H__

#include "interface.h"
#include "hmac.h"
#include "treestate.h" 
#include "tree.h" 

#ifdef  __cplusplus
extern "C" {
#endif

#define MAX_VERSION 3
#define VERSION_BITS(x) (x>>6) & MAX_VERSION

typedef struct securityLayerImplementation{
  unsigned char * (*parseHeader)(unsigned char* , size_t , unsigned int *);

  void * (*getHeader)(unsigned int*);
  void * (*getPermCode)(unsigned int*);
  STATE_INDEX_TYPE (*getSecretIndex)(void);
  STATE_INDEX_TYPE (*getPermIndex)(void);
  STATE_INDEX_TYPE (*getKeyIndex)(void);
  
  unsigned int (*setHeader)(unsigned char* , size_t);
  unsigned int (*setPermCode)(unsigned char* , size_t);
  unsigned int (*setSecretIndex)(unsigned char* , size_t);
  unsigned int (*setPermIndex)(unsigned char* , size_t);
  unsigned int (*setKeyIndex)(unsigned char* , size_t);
 
  unsigned int MACsize;
}securityLayerImplementation;


int handleSecurityLayer(uint8_t, unsigned char *, size_t, unsigned int*);
int handleSecurityLayerAfterPayload(uint8_t, unsigned char *, size_t, unsigned char* res, size_t *);


#ifdef  __cplusplus
}
#endif /* __cplusplus */

#endif /* __SECURITY_LAYER_H__ */
