/*-
 * securitylayerhandler.c - Security layer handler 
 *
 * Copyright 2005 Wenwen Chen
*/

/*!
 * \addtogroup    handler 
 * \{
 *
 * \file
 * \brief       Header definitions for the security layer functions
 *
 * \author      Wenwen Chen 
 */
#include "securitylayer.h"
#include "securitylayerV0.h"
//#include "securitylayerV1.h"

const static securityLayerImplementation securityLayerImplementationV0 = {
  .parseHeader = parseHeaderV0,
  .generateHeader = generateHeaderV0,
  .generatePermCode = generatePermCodeV0,
  .getHeader = getHeaderV0,
  .getPermCode = getPermCodeV0,
  .getSecretIndex = getSecretIndexV0,
  .getPermIndex = getPermIndexV0,
  .getKeyIndex = getKeyIndexV0,
  .setPermCode = setPermCodeV0,
  .setSecretIndex = setSecretIndexV0,
  .setPermIndex = setPermIndexV0,
  .setKeyIndex = setKeyIndexV0,
  .MACsize = MAC_LEN_V0
};

// TODO
//const static securityLayerImplementation securityLayerImplementationV1 = {
//  .parseHeader = setHeaderV1,
//  .getSecretIndex = getSecretIndexV1,
//  .getPermIndex = getPermIndexV1,
//  .getKeyIndex = getKeyIndexV1,
//  .setSecretIndex = setSecretIndexV1,
//  .setPermIndex = setPermIndexV1,
//  .setKeyIndex = setKeyIndexV1,
//  .MACsize = MAC_LEN_V1,
//};

const securityLayerImplementation* implementations[SECURITY_LAYER_IMPLEMENTATIONS_LEN] = {&securityLayerImplementationV0};

unsigned char tmpMAC[HASH_SIZE] = {0};

// TODO fix me for clients
static inline tree_node * getKeyNode(STATE_TYPE secret_index, unsigned char * perm_code, unsigned int perm_code_size, STATE_TYPE key_index, unsigned int flag){
  
  tree_edge * edges = getEdges(2);
  if(edges == NULL){
    return NULL;
  }

  edges[0].func = edgeFunc;
  edges[1].func = edgeFunc;

  edges[0].params = perm_code;
  edges[0].params_size = perm_code_size;


  edges[1].params = (unsigned char *)(&key_index);
  edges[1].params_size = STATE_SIZE;

  if(flag){
    return fillNodes(getPathFromRoot(3), edges, 3, flag);
  }else{
    return fillNodes(getPathFromRootThenCachedNodes(3, secret_index), edges, 3, flag);
  }
}


static inline int verifyMAC(tree_node * key, unsigned char * msg, unsigned int *p_msg_size, unsigned int mac_size){
  unsigned int size = 0;
  *p_msg_size -= mac_size;
  hmac(&sha_construction, key->block, key->size, msg, *p_msg_size, tmpMAC, &size);

  return memcmp(tmpMAC, msg+*p_msg_size, (unsigned int)mac_size);
}

/*!
 *\return security descriptor id (index)
 */
unsigned int handleSecurityLayer(unsigned char *msg, unsigned int * p_msg_size, unsigned int *p_header_size){
  uint8_t currVersion = SECURITY_LAYER_VERSION_BITS(msg[0]);

  if(currVersion>SECURITY_LAYER_IMPLEMENTATIONS_LEN){
    *p_header_size = 1;
    *p_msg_size--;
    return NO_DESCRIPTOR;
  }
  const securityLayerImplementation * implementation = implementations[currVersion];
  if(NULL == implementation){
    *p_header_size = 1;
    *p_msg_size--;
    return NO_DESCRIPTOR;
  }
  unsigned int descriptor = createSecurityDescriptor(); 
  if(0 == (*p_header_size = implementation->parseHeader(descriptor, msg, p_msg_size))){
    return NO_DESCRIPTOR;
  }

  unsigned int perm_code_size = 0;
  unsigned char* perm_code = implementation->getPermCode(descriptor, &perm_code_size);
  STATE_TYPE perm_index = implementation->getPermIndex(descriptor);
  STATE_TYPE secret_index = implementation->getSecretIndex(descriptor);
  STATE_TYPE indexes [2] = {perm_index,  implementation->getKeyIndex(descriptor)};
  unsigned int forceUpdate = (getExpectedState(secret_index, 0) != perm_index);
  if(SUCC == updateExpectedStateVector(secret_index, indexes, 1)){
    tree_node * currKeyNode = getKeyNode(secret_index, perm_code, perm_code_size, indexes[1], forceUpdate);
    if(0 == verifyMAC(currKeyNode, msg+*p_header_size, p_msg_size, implementation->MACsize)){
      return activeSecurityDescriptor(updateSecurityDescriptor(descriptor, currVersion, currKeyNode));
    }else{
      PRINT("[ERROR] Failed to verify MAC!\n"); 
    }
  }else{
    PRINT("[ERROR] Failed to update state!\n");
  }

  return NO_DESCRIPTOR;
}
/*!
 * \}
 */

