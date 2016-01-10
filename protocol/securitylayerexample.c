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

const uint8_t PERMCODE_TYPE = 0x40;
static securityHeaderExample currHeaders[SECURITY_DESCRIPTORS_LEN+SECURITY_PREDEF_LEN] = {0};

unsigned int parseHeaderExample(unsigned int security_descriptor, unsigned char * msg, unsigned int *p_msg_size){
  if(security_descriptor>=SECURITY_DESCRIPTORS_LEN+SECURITY_PREDEF_LEN || msg[0]>>6 != SECURITY_LAYER_TYPE_EXAMPLE || *p_msg_size < SECURITY_LAYER_HEADER_LEN_EXAMPLE + MAC_LEN_EXAMPLE){
    return 0;
  }
  * p_msg_size -= SECURITY_LAYER_HEADER_LEN_EXAMPLE;
  memcpy(currHeaders+security_descriptor, msg, SECURITY_LAYER_HEADER_LEN_EXAMPLE);
  return SECURITY_LAYER_HEADER_LEN_EXAMPLE;
}

unsigned int generateHeaderExample(unsigned int security_descriptor, unsigned char* msg, unsigned int msg_size){
  if(security_descriptor>=SECURITY_DESCRIPTORS_LEN+SECURITY_PREDEF_LEN || msg_size < SECURITY_LAYER_HEADER_LEN_EXAMPLE){
    return 0;
  }
  // FIXME not static
  currHeaders[security_descriptor].perm_code.permcode_type=PERMCODE_TYPE;
  memcpy(msg, currHeaders+security_descriptor, SECURITY_LAYER_HEADER_LEN_EXAMPLE);
  return SECURITY_LAYER_HEADER_LEN_EXAMPLE;
}

unsigned int generatePermCodeExample(unsigned int security_descriptor, unsigned char* msg, unsigned int msg_size){
  if(security_descriptor>=SECURITY_DESCRIPTORS_LEN+SECURITY_PREDEF_LEN || msg_size < SECURITY_LAYER_PERMCODE_LEN_EXAMPLE){
    return 0;
  }
  // FIXME not static
  currHeaders[security_descriptor].perm_code.permcode_type=PERMCODE_TYPE;
  memcpy(msg, &(currHeaders[security_descriptor].perm_code), SECURITY_LAYER_PERMCODE_LEN_EXAMPLE);
  return SECURITY_LAYER_PERMCODE_LEN_EXAMPLE;
}

void * getHeaderExample(unsigned int security_descriptor, unsigned int *size){
  if(security_descriptor>=SECURITY_DESCRIPTORS_LEN+SECURITY_PREDEF_LEN){
    return NULL;
  }
  if(size != NULL)
  *size = SECURITY_LAYER_HEADER_LEN_EXAMPLE;
  return currHeaders+security_descriptor;
} 

void * getPermCodeExample(unsigned int security_descriptor, unsigned int * size){
  if(security_descriptor>=SECURITY_DESCRIPTORS_LEN+SECURITY_PREDEF_LEN){
    return NULL;
  }
  if(size != NULL)
    *size = sizeof(permCodeExample);
  // FIXME not static
  currHeaders[security_descriptor].perm_code.permcode_type=PERMCODE_TYPE;
  return &(currHeaders[security_descriptor].perm_code);
}

TREE_STATE_TYPE getSecretIndexExample(unsigned int security_descriptor){
  if(security_descriptor>=SECURITY_DESCRIPTORS_LEN+SECURITY_PREDEF_LEN){
    return NO_TREE_STATE;
  }
  return (TREE_STATE_TYPE)((currHeaders[security_descriptor].perm_code).secret_index);
}

TREE_STATE_TYPE getPermIndexExample(unsigned int security_descriptor){
  if(security_descriptor>=SECURITY_DESCRIPTORS_LEN+SECURITY_PREDEF_LEN){
    return NO_TREE_STATE;
  }
  return (TREE_STATE_TYPE)((currHeaders[security_descriptor].perm_code).perm_index);
}

RIGHT_TYPE getPermExample(unsigned int security_descriptor){
  if(security_descriptor>=SECURITY_DESCRIPTORS_LEN+SECURITY_PREDEF_LEN){
    return NO_RIGHT;
  }
  return (TREE_STATE_TYPE)((currHeaders[security_descriptor].perm_code).perm);
}

TREE_STATE_TYPE getKeyIndexExample(unsigned int security_descriptor){
  if(security_descriptor>=SECURITY_DESCRIPTORS_LEN+SECURITY_PREDEF_LEN){
    return NO_TREE_STATE;
  }
  return (TREE_STATE_TYPE)(currHeaders[security_descriptor].key_index);
}

unsigned int setSecretIndexExample(unsigned int security_descriptor, TREE_STATE_TYPE index){
  if(security_descriptor >= SECURITY_DESCRIPTORS_LEN+SECURITY_PREDEF_LEN){
    return 0;
  }
  (currHeaders[security_descriptor].perm_code).secret_index = (uint8_t)index;
  return 1;
}

unsigned int setPermIndexExample(unsigned int security_descriptor, TREE_STATE_TYPE index){
  if(security_descriptor >= SECURITY_DESCRIPTORS_LEN+SECURITY_PREDEF_LEN){
    return 0;
  }
  (currHeaders[security_descriptor].perm_code).perm_index = (uint8_t)index;
  return 1;
}

unsigned int setPermExample(unsigned int security_descriptor, RIGHT_TYPE perm_right){
  if(security_descriptor >= SECURITY_DESCRIPTORS_LEN+SECURITY_PREDEF_LEN){
    return 0;
  }
  (currHeaders[security_descriptor].perm_code).perm = (uint8_t)perm_right;
  return 1;
}

unsigned int setKeyIndexExample(unsigned int security_descriptor, TREE_STATE_TYPE index){
  if(security_descriptor >= SECURITY_DESCRIPTORS_LEN+SECURITY_PREDEF_LEN){
    return 0;
  }
  currHeaders[security_descriptor].key_index = (uint8_t)index;
  return 1;
}

unsigned int setPermCodeExample(unsigned int security_descriptor, unsigned char *code, unsigned int code_size){
  if(security_descriptor >= SECURITY_DESCRIPTORS_LEN+SECURITY_PREDEF_LEN){
    return 0;
  }
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

