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
  unsigned char * (*parseHeader)(unsigned char* , unsigned int* , unsigned int *);

  void * (*getHeader)(unsigned int*);
  void * (*getPermCode)(unsigned int*);
  STATE_INDEX_TYPE (*getSecretIndex)(void);
  STATE_INDEX_TYPE (*getPermIndex)(void);
  STATE_INDEX_TYPE (*getKeyIndex)(void);
  
  unsigned int (*setHeader)(unsigned char* , unsigned int);
  unsigned int (*setPermCode)(unsigned char* , unsigned int);
  unsigned int (*setSecretIndex)(unsigned char* , unsigned int);
  unsigned int (*setPermIndex)(unsigned char* , unsigned int);
  unsigned int (*setKeyIndex)(unsigned char* , unsigned int);
 
  unsigned int MACsize;
}securityLayerImplementation;


int handleSecurityLayer(uint8_t, unsigned char *, unsigned int*, unsigned int*);
int handleSecurityLayerAfterPayload(uint8_t, unsigned char *, unsigned int, unsigned char* res, unsigned int *);
void clearSecurityLayerSession(unsigned int);

void resetKey(void);


#ifdef  __cplusplus
}
#endif /* __cplusplus */

#endif /* __SECURITY_LAYER_H__ */
