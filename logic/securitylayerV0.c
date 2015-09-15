#include "securitylayerV0.h"
#include <string.h>

static securityHeaderV0 currHeaders[MAX_SECURITY_DESCRIPTOR] = {0};

unsigned int parseHeaderV0(unsigned int security_descriptor, unsigned char * msg, unsigned int *p_msg_size){
  if(msg[0]>>6 != SECURITY_LAYER_VERSION_V0 || *p_msg_size < SECURITY_LAYER_HEADER_LEN_V0 + MAC_LEN_V0){
    return 0;
  }
  * p_msg_size -= SECURITY_LAYER_HEADER_LEN_V0;
  memcpy(currHeaders+security_descriptor, msg, SECURITY_LAYER_HEADER_LEN_V0);
  return SECURITY_LAYER_HEADER_LEN_V0;
}

void * getHeaderV0(unsigned int security_descriptor, unsigned int *size){
  *size = SECURITY_LAYER_HEADER_LEN_V0;
  return currHeaders+security_descriptor;
} 

void * getPermCodeV0(unsigned int security_descriptor, unsigned int * size){
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

unsigned int setHeaderV0(unsigned int security_descriptor, unsigned char* msg, unsigned int msg_size){
  if(msg_size < SECURITY_LAYER_HEADER_LEN_V0){
    return 0;
  }
  memcpy(msg, currHeaders+security_descriptor, SECURITY_LAYER_HEADER_LEN_V0);
  return SECURITY_LAYER_HEADER_LEN_V0;
}

unsigned int setSecretIndexV0(unsigned int security_descriptor, unsigned char* msg, unsigned int msg_size){
  if(msg_size < 1){
    return 0;
  }
  msg[0] = (unsigned char)((currHeaders[security_descriptor].perm_code).secret_index);
  return 1;
}

unsigned int setPermCodeV0(unsigned int security_descriptor, unsigned char* msg, unsigned int msg_size){
  unsigned int size = sizeof(permCodeV0);
  if(msg_size < size){
    return 0;
  }
  memcpy(msg, &(currHeaders[security_descriptor].perm_code), size);
  return size;
}

unsigned int setPermIndexV0(unsigned int security_descriptor, unsigned char* msg, unsigned int msg_size){
  if(msg_size < 1){
    return 0;
  }
  msg[0] = (unsigned char)((currHeaders[security_descriptor].perm_code).perm_index);
  return 1;
}

unsigned int setKeyIndexV0(unsigned int security_descriptor, unsigned char* msg, unsigned int msg_size){
  if(msg_size < 1){
    return 0;
  }
  msg[0] = (unsigned char)(currHeaders[security_descriptor].key_index);
  return 1;
}
