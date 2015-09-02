#ifndef __SECURITY_LAYER_V0_H__
#define __SECURITY_LAYER_V0_H__

#include <stdint.h>
#include <stddef.h>

#include "treestate.h"
#ifdef  __cplusplus
extern "C" {
#endif


#define HEADER_LEN_V0 4
#define MAC_LEN_V0 20

#pragma pack(1)
  typedef struct securityHeaderV0{
    uint8_t indicator;
    uint8_t secret_index;
    uint8_t perm_index;
    uint8_t key_index;
  }securityHeaderV0;
#pragma pack()

  unsigned char * setHeaderV0(unsigned char* , size_t , uint8_t *);

  STATE_INDEX_TYPE getSecretIndexV0(void);
  STATE_INDEX_TYPE getPermIndexV0(void);
  STATE_INDEX_TYPE getKeyIndexV0(void);

  uint8_t setSecretIndexV0(unsigned char* , size_t);
  uint8_t setPermIndexV0(unsigned char* , size_t);
  uint8_t setKeyIndexV0(unsigned char* , size_t);

#ifdef  __cplusplus
}
#endif /* __cplusplus */

#endif /* __SECURITY_LAYER_V0_H__ */
