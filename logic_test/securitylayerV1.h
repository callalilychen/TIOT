#ifndef __SECURITY_LAYER_V1_H__
#define __SECURITY_LAYER_V1_H__

#include <stdint.h> 
#include <stddef.h>

#include "treestate.h"

#ifdef  __cplusplus
extern "C" {
#endif


#define MAC_LEN_V1 20

typedef struct police{
  uint64_t code;
} police;

typedef struct timestamps{
  uint32_t time;
}timestamps;

#define AS_ID(x) x&0x80000000
#define RS_ID(x) x&0x40000000
#define SIL(x) (x>>12) & 0x3
#define PIL(x) (x>>10) & 0x3
#define KIL(x) (x>>8) & 0x3
#define APL(x) (x>>6) & 0x3
#define TL(x) (x>>4) & 0x3
#define PF(x) x & 0x1
typedef struct token{
  uint16_t len;
  uint16_t as_id;
  uint16_t rs_id;
  STATE_INDEX_TYPE secret_index;
  STATE_INDEX_TYPE perm_index;
  timestamps time;
  police polices;
  uint8_t padding;
}token;

#define AUTHORIZATION_TOKEN_LEN(x) (x>>1) & 0x1f 
#define CIPHER_FLAG(x) x & 0x1
typedef struct securityHeaderV1{
  uint8_t indicator;
  token authorization_token;
  STATE_INDEX_TYPE key_index;
  uint8_t cipher;
  uint8_t res_cipher;
}securityHeaderV1;

#define MAC_LEN 20

typedef struct message{
  uint8_t header_len;
  securityHeaderV1 header;
  unsigned char* p_payload;
  uint8_t payload_len;
  unsigned char mac[MAC_LEN];
}message;

  unsigned char * setHeaderV1(unsigned char* , size_t , uint8_t *);

  STATE_INDEX_TYPE getSecretIndexV1(void);
  STATE_INDEX_TYPE getPermIndexV1(void);
  STATE_INDEX_TYPE getKeyIndexV1(void);

  uint8_t setSecretIndexV1(unsigned char* , size_t);
  uint8_t setPermIndexV1(unsigned char* , size_t);
  uint8_t setKeyIndexV1(unsigned char* , size_t);

#ifdef  __cplusplus
}
#endif /* __cplusplus */

#endif /* __SECURITY_LAYER_V1_H__ */
