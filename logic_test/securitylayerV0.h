#ifndef __SECURITY_LAYER_V0_H__
#define __SECURITY_LAYER_V0_H__

#include <stddef.h>

#include "treestate.h"
#ifdef  __cplusplus
extern "C" {
#endif


#define HEADER_LEN_V0 4
#define MAC_LEN_V0 20

#pragma pack(1)
  typedef struct permCodeV0{
    uint8_t secret_index;
    uint8_t perm_index;
  }permCodeV0;

  typedef struct securityHeaderV0{
    uint8_t indicator;
    permCodeV0 perm_code;
    uint8_t key_index;
  }securityHeaderV0;
#pragma pack()

  unsigned char * parseHeaderV0(unsigned char* , size_t , unsigned int *);
  
  void * getHeaderV0(unsigned int *);
  void * getPermCodeV0(unsigned int *);
  STATE_INDEX_TYPE getSecretIndexV0(void);
  STATE_INDEX_TYPE getPermIndexV0(void);
  STATE_INDEX_TYPE getKeyIndexV0(void);

  unsigned int setHeaderV0(unsigned char* , size_t);
  unsigned int setPermCodeV0(unsigned char* , size_t);
  unsigned int setSecretIndexV0(unsigned char* , size_t);
  unsigned int setPermIndexV0(unsigned char* , size_t);
  unsigned int setKeyIndexV0(unsigned char* , size_t);

#ifdef  __cplusplus
}
#endif /* __cplusplus */

#endif /* __SECURITY_LAYER_V0_H__ */
