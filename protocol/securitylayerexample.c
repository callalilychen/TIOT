/*-
 * securitylayerexample.c - Security layer protocol example with type 0 
 *
 * Copyright 2015 Wenwen Chen
*/

/*!
 * \addtogroup    securitylayerprotocol
 * \{
 *
 * \file
 * \brief       Implementation of the security layer protocol example with type 0
 *
 * \author      Wenwen Chen 
 */
#include "securitylayerexample.h"
#include <string.h>

static securityHeaderExample currHeaders[SECURITY_DESCRIPTORS_LEN] = {0};

unsigned int parseHeaderExample(unsigned int security_descriptor, unsigned char * msg, unsigned int *p_msg_size){
  if(msg[0]>>6 != SECURITY_LAYER_TYPE_EXAMPLE || *p_msg_size < SECURITY_LAYER_HEADER_LEN_EXAMPLE + MAC_LEN_EXAMPLE){
    return 0;
  }
  * p_msg_size -= SECURITY_LAYER_HEADER_LEN_EXAMPLE;
  memcpy(currHeaders+security_descriptor, msg, SECURITY_LAYER_HEADER_LEN_EXAMPLE);
  return SECURITY_LAYER_HEADER_LEN_EXAMPLE;
}

unsigned int generateHeaderExample(unsigned int security_descriptor, unsigned char* msg, unsigned int msg_size){
  if(msg_size < SECURITY_LAYER_HEADER_LEN_EXAMPLE){
    return 0;
  }
  memcpy(msg, currHeaders+security_descriptor, SECURITY_LAYER_HEADER_LEN_EXAMPLE);
  return SECURITY_LAYER_HEADER_LEN_EXAMPLE;
}

unsigned int generatePermCodeExample(unsigned int security_descriptor, unsigned char* msg, unsigned int msg_size){
  if(msg_size < SECURITY_LAYER_PERMCODE_LEN_EXAMPLE){
    return 0;
  }
  memcpy(msg, &(currHeaders[security_descriptor].perm_code), SECURITY_LAYER_PERMCODE_LEN_EXAMPLE);
  return SECURITY_LAYER_PERMCODE_LEN_EXAMPLE;
}

void * getHeaderExample(unsigned int security_descriptor, unsigned int *size){
  if(size != NULL)
  *size = SECURITY_LAYER_HEADER_LEN_EXAMPLE;
  return currHeaders+security_descriptor;
} 

void * getPermCodeExample(unsigned int security_descriptor, unsigned int * size){
  if(size != NULL)
  *size = sizeof(permCodeExample);
  return &(currHeaders[security_descriptor].perm_code);
}

TREE_STATE_TYPE getSecretIndexExample(unsigned int security_descriptor){
  return (TREE_STATE_TYPE)((currHeaders[security_descriptor].perm_code).secret_index);
}

TREE_STATE_TYPE getPermIndexExample(unsigned int security_descriptor){
  return (TREE_STATE_TYPE)((currHeaders[security_descriptor].perm_code).perm_index);
}

RIGHT_TYPE getPermExample(unsigned int security_descriptor){
  return (TREE_STATE_TYPE)((currHeaders[security_descriptor].perm_code).perm);
}

TREE_STATE_TYPE getKeyIndexExample(unsigned int security_descriptor){
  return (TREE_STATE_TYPE)(currHeaders[security_descriptor].key_index);
}

void setSecretIndexExample(unsigned int security_descriptor, TREE_STATE_TYPE index){
  (currHeaders[security_descriptor].perm_code).secret_index = index;
}

void setPermIndexExample(unsigned int security_descriptor, TREE_STATE_TYPE index){
  (currHeaders[security_descriptor].perm_code).perm_index = index;
}

void setPermExample(unsigned int security_descriptor, RIGHT_TYPE perm_right){
  (currHeaders[security_descriptor].perm_code).perm = perm_right;
}

void setKeyIndexExample(unsigned int security_descriptor, TREE_STATE_TYPE index){
  currHeaders[security_descriptor].key_index = index;
}

unsigned int setPermCodeExample(unsigned int security_descriptor, unsigned char *code, unsigned int code_size){
  unsigned int size = sizeof(permCodeExample);
  if(code_size < size){
    memcpy(&(currHeaders[security_descriptor].perm_code), code, code_size);
    for(int i = code_size; i < size; i++){
      ((int8_t *)(&currHeaders[security_descriptor].perm_code))[i] = 0;  
    }
    return code_size;
  }
    memcpy(&(currHeaders[security_descriptor].perm_code), code, size);
  return size;
}
/*!
 * \}
 */

