#ifndef __SECURITY_LAYER_V0_H__
#define __SECURITY_LAYER_V0_H__

#include "treestate.h"
#include "protocolconfig.h"
#ifdef  __cplusplus
extern "C" {
#endif

#define SECURITY_LAYER_VERSION_V0 0
#define SECURITY_LAYER_HEADER_LEN_V0 4
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

  unsigned int parseHeaderV0(unsigned int security_descriptor, unsigned char * msg, unsigned int *p_msg_size);

  void * getHeaderV0(unsigned int security_descriptor, unsigned int *size);
  void * getPermCodeV0(unsigned int security_descriptor, unsigned int * size);
  STATE_TYPE getSecretIndexV0(unsigned int security_descriptor);
  STATE_TYPE getPermIndexV0(unsigned int security_descriptor);
  STATE_TYPE getKeyIndexV0(unsigned int security_descriptor);

  unsigned int setHeaderV0(unsigned int security_descriptor, unsigned char* msg, unsigned int msg_size);
  unsigned int setSecretIndexV0(unsigned int security_descriptor, unsigned char* msg, unsigned int msg_size);
  unsigned int setPermCodeV0(unsigned int security_descriptor, unsigned char* msg, unsigned int msg_size);
  unsigned int setPermIndexV0(unsigned int security_descriptor, unsigned char* msg, unsigned int msg_size);
  unsigned int setKeyIndexV0(unsigned int security_descriptor, unsigned char* msg, unsigned int msg_size);

#ifdef  __cplusplus
}
#endif /* __cplusplus */

#endif /* __SECURITY_LAYER_V0_H__ */
