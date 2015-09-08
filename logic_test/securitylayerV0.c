#include "securitylayerV0.h"
#include <string.h>

// TODO session
securityHeaderV0 * currHeader = NULL;

unsigned char * parseHeaderV0(unsigned char * msg, unsigned int *p_msg_size, unsigned int *p_header_size){
  if(msg[0]>>6 || *p_msg_size < HEADER_LEN_V0 + MAC_LEN_V0){
    * p_header_size = 0;
    return NULL;
  }
  currHeader = (securityHeaderV0 *)msg;
  * p_header_size = HEADER_LEN_V0;
  * p_msg_size -= HEADER_LEN_V0;
  return (unsigned char *)currHeader;
}

void * getHeaderV0(unsigned int * size){
  *size = HEADER_LEN_V0;
  return currHeader;
} 

void * getPermCodeV0(unsigned int * size){
  *size = sizeof(permCodeV0);
  return &(currHeader->perm_code);
}

STATE_INDEX_TYPE getSecretIndexV0(){
  return (STATE_INDEX_TYPE)((currHeader->perm_code).secret_index);
}

STATE_INDEX_TYPE getPermIndexV0(){
  return (STATE_INDEX_TYPE)((currHeader->perm_code).perm_index);
}

STATE_INDEX_TYPE getKeyIndexV0(){
  return (STATE_INDEX_TYPE)(currHeader->key_index);
}

unsigned int setHeaderV0(unsigned char* msg, unsigned int msg_size){
  if(msg_size < HEADER_LEN_V0){
    return 0;
  }
  memcpy(msg, currHeader, HEADER_LEN_V0);
  return HEADER_LEN_V0;
}

unsigned int setSecretIndexV0(unsigned char* msg, unsigned int msg_size){
  unsigned int size = sizeof(permCodeV0);
  if(msg_size < size){
    return 0;
  }
  memcpy(msg, &(currHeader->perm_code), size);
  return size;
}

unsigned int setPermCodeV0(unsigned char* msg, unsigned int msg_size){
  if(msg_size < 1){
    return 0;
  }
  msg[0] = (unsigned char)((currHeader->perm_code).secret_index);
  return 1;
}

unsigned int setPermIndexV0(unsigned char* msg, unsigned int msg_size){
  if(msg_size < 1){
    return 0;
  }
  msg[0] = (unsigned char)((currHeader->perm_code).perm_index);
  return 1;
}

unsigned int setKeyIndexV0(unsigned char* msg, unsigned int msg_size){
  if(msg_size < 1){
    return 0;
  }
  msg[0] = (unsigned char)(currHeader->key_index);
  return 1;
}
