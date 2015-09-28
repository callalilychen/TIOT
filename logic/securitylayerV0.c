#include "securitylayerV0.h"
#include <string.h>

static securityHeaderV0 currHeaders[SECURITY_DESCRIPTORS_LEN] = {0};

unsigned int parseHeaderV0(unsigned int security_descriptor, unsigned char * msg, unsigned int *p_msg_size){
  if(msg[0]>>6 != SECURITY_LAYER_VERSION_V0 || *p_msg_size < SECURITY_LAYER_HEADER_LEN_V0 + MAC_LEN_V0){
    return 0;
  }
  * p_msg_size -= SECURITY_LAYER_HEADER_LEN_V0;
  memcpy(currHeaders+security_descriptor, msg, SECURITY_LAYER_HEADER_LEN_V0);
  return SECURITY_LAYER_HEADER_LEN_V0;
}

unsigned int generateHeaderV0(unsigned int security_descriptor, unsigned char* msg, unsigned int msg_size){
  if(msg_size < SECURITY_LAYER_HEADER_LEN_V0){
    return 0;
  }
  memcpy(msg, currHeaders+security_descriptor, SECURITY_LAYER_HEADER_LEN_V0);
  return SECURITY_LAYER_HEADER_LEN_V0;
}

unsigned int generatePermCodeV0(unsigned int security_descriptor, unsigned char* msg, unsigned int msg_size){
  if(msg_size < SECURITY_LAYER_PERMCODE_LEN_V0){
    return 0;
  }
  memcpy(msg, &(currHeaders[security_descriptor].perm_code), SECURITY_LAYER_PERMCODE_LEN_V0);
  return SECURITY_LAYER_PERMCODE_LEN_V0;
}

void * getHeaderV0(unsigned int security_descriptor, unsigned int *size){
  if(size != NULL)
  *size = SECURITY_LAYER_HEADER_LEN_V0;
  return currHeaders+security_descriptor;
} 

void * getPermCodeV0(unsigned int security_descriptor, unsigned int * size){
  if(size != NULL)
  *size = sizeof(permCodeV0);
  return &(currHeaders[security_descriptor].perm_code);
}

STATE_TYPE getSecretIndexV0(unsigned int security_descriptor){
  return (STATE_TYPE)((currHeaders[security_descriptor].perm_code).secret_index);
}

STATE_TYPE getPermIndexV0(unsigned int security_descriptor){
  return (STATE_TYPE)((currHeaders[security_descriptor].perm_code).perm_index);
}

STATE_TYPE getKeyIndexV0(unsigned int security_descriptor){
  return (STATE_TYPE)(currHeaders[security_descriptor].key_index);
}

void setSecretIndexV0(unsigned int security_descriptor, STATE_TYPE index){
  (currHeaders[security_descriptor].perm_code).secret_index = index;
}

void setPermIndexV0(unsigned int security_descriptor, STATE_TYPE index){
  (currHeaders[security_descriptor].perm_code).perm_index = index;
}

void setKeyIndexV0(unsigned int security_descriptor, STATE_TYPE index){
  currHeaders[security_descriptor].key_index = index;
}

unsigned int setPermCodeV0(unsigned int security_descriptor, unsigned char *code, unsigned int code_size){
  unsigned int size = sizeof(permCodeV0);
  if(code_size < size){
    bzero(&(currHeaders[security_descriptor].perm_code), size);
    memcpy(&(currHeaders[security_descriptor].perm_code), code, code_size);
    return code_size;
  }
    memcpy(&(currHeaders[security_descriptor].perm_code), code, size);
  return size;
}

