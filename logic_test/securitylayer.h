#ifndef __SECURITY_LAYER_H__
#define __SECURITY_LAYER_H__

#include "interface.h"
#include "treestate.h" 

#ifdef  __cplusplus
extern "C" {
#endif

#define MAX_VERSION 3
#define VERSION_BITS(x) (x>>6) & MAX_VERSION

typedef struct securityLayerImplementation{
  unsigned char * (*setHeader)(unsigned char* , size_t , uint8_t *);

  STATE_INDEX_TYPE (*getSecretIndex)(void);
  STATE_INDEX_TYPE (*getPermIndex)(void);
  STATE_INDEX_TYPE (*getKeyIndex)(void);

  uint8_t (*setSecretIndex)(unsigned char* , size_t);
  uint8_t (*setPermIndex)(unsigned char* , size_t);
  uint8_t (*setKeyIndex)(unsigned char* , size_t);
 
  uint8_t MACsize;
}securityLayerImplementation;


int handleSecurityLayer(uint8_t, unsigned char *, size_t, unsigned char **, size_t *, unsigned char* res, size_t *);
int handleSecurityLayerAfterPayload(uint8_t, unsigned char *, size_t, unsigned char* res, size_t *);


#ifdef  __cplusplus
}
#endif /* __cplusplus */

#endif /* __SECURITY_LAYER_H__ */
