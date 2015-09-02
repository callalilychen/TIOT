#include "securitylayerV0.h"

static securityHeaderV0 * currHeader = NULL;

unsigned char * setHeaderV0(unsigned char * msg, size_t msg_size, uint8_t *p_header_size){
  if(msg[0] || msg_size < HEADER_LEN_V0 + MAC_LEN_V0){
    * p_header_size = 0;
    return NULL;
  }
  currHeader = (securityHeaderV0 *)msg;
  * p_header_size = HEADER_LEN_V0;
  return currHeader;
}

STATE_INDEX_TYPE getSecretIndexV0(){
  return (STATE_INDEX_TYPE)(currHeader->secret_index);
}

STATE_INDEX_TYPE getPermIndexV0(){
  return (STATE_INDEX_TYPE)(currHeader->perm_index);
}

STATE_INDEX_TYPE getKeyIndexV0(){
  return (STATE_INDEX_TYPE)(currHeader->key_index);
}

uint8_t setSecretIndexV0(unsigned char* msg, size_t msg_size){
  if(msg_size < 1){
    return 0;
  }
  msg[0] = (unsigned char)(currHeader->secret_index);
  return 1;
}

uint8_t setPermIndexV0(unsigned char* msg, size_t msg_size){
  if(msg_size < 1){
    return 0;
  }
  msg[0] = (unsigned char)(currHeader->perm_index);
  return 1;
}

uint8_t setKeyIndexV0(unsigned char* msg, size_t msg_size){
  if(msg_size < 1){
    return 0;
  }
  msg[0] = (unsigned char)(currHeader->key_index);
  return 1;
}
