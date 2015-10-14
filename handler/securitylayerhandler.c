/*-
 * securitylayerhandler.c - Security layer handler 
 *
 * Copyright 2015 Wenwen Chen
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
#include "securitylayerhandler.h"
#include "securitylayerexample.h"
//#include "securitylayerV1.h"

const static securityLayerImplementation securityLayerImplementationV0 = {
  .parseHeader = parseHeaderExample,
  .generateHeader = generateHeaderExample,
  .generatePermCode = generatePermCodeExample,
  .getHeader = getHeaderExample,
  .getPermCode = getPermCodeExample,
  .getSecretIndex = getSecretIndexExample,
  .getPermIndex = getPermIndexExample,
  .getPerm = getPermExample,
  .getKeyIndex = getKeyIndexExample,
  .setPermCode = setPermCodeExample,
  .setSecretIndex = setSecretIndexExample,
  .setPermIndex = setPermIndexExample,
  .setPerm = setPermExample,
  .setKeyIndex = setKeyIndexExample,
  .HeaderSize = SECURITY_LAYER_HEADER_LEN_EXAMPLE,
  .MACsize = MAC_LEN_EXAMPLE
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
descriptor_security tmpDescriptorSecurity = {0};

// TODO fix me for clients
static inline tree_node * getKeyNode(TREE_STATE_TYPE secret_index, unsigned char * perm_code, unsigned int perm_code_size, TREE_STATE_TYPE key_index, unsigned int flag){
  
  tree_edge * edges = getEdgesWithFunction(2, edgeFunc);
  if(edges == NULL){
    return NULL;
  }
  edges[0].params = perm_code;
  edges[0].params_size = perm_code_size;

  edges[1].params = (unsigned char *)(&key_index);
  edges[1].params_size = TREE_STATE_SIZE;

  if(flag){
    return fillNodes(getPathFromRoot(2), edges, 3, flag);
  }else{
    return fillNodes(getPathFromRootThenCachedNodes(2, secret_index), edges, 3, flag);
  }
}


static inline int verifyMAC(tree_node * key, unsigned char * msg, unsigned int *p_msg_size, unsigned int mac_size){
  unsigned int size = 0;
  *p_msg_size -= mac_size;
  hmac(&sha_construction, key->block, key->size, msg, *p_msg_size, tmpMAC, &size);

  return 0==memcmp(tmpMAC, msg+*p_msg_size, (unsigned int)mac_size);
}

/*!
 *\return security descriptor_id id (index)
 */
unsigned int handleSecurityLayer(unsigned char *msg, unsigned int * p_msg_size, unsigned int *p_header_size){
  tmpDescriptorSecurity.protocol_type = SECURITY_LAYER_PROTOCOL_TYPE_BITS(msg[0]);

  *p_header_size = 0;
  if(tmpDescriptorSecurity.protocol_type>SECURITY_LAYER_IMPLEMENTATIONS_LEN){
    return NO_DESCRIPTOR;
  }
  const securityLayerImplementation * implementation = implementations[tmpDescriptorSecurity.protocol_type];
  if(NULL == implementation){
    return NO_DESCRIPTOR;
  }
  unsigned int descriptor_id = getLeastActiveSecurityDescriptor(); 
  if(0 == (*p_header_size = implementation->parseHeader(descriptor_id, msg, p_msg_size))){
    return NO_DESCRIPTOR;
  }

  unsigned int perm_code_size = 0;
  unsigned char* perm_code = implementation->getPermCode(descriptor_id, &perm_code_size);
  TREE_STATE_TYPE perm_index = implementation->getPermIndex(descriptor_id);
  TREE_STATE_TYPE secret_index = implementation->getSecretIndex(descriptor_id);
  TREE_STATE_TYPE indexes [2] = {perm_index,  implementation->getKeyIndex(descriptor_id)};
  unsigned int forceUpdate = (getExpectedState(secret_index, 0) != perm_index);
  //TODO Work around to let AS not update state, perhaps replace with method to wait a message with the same key
#ifdef SECURITY_LAYER_UPDATE_STATE
    PRINT("%s Update state!\n", INFO_MESSAGE);
  if(SUCC == updateExpectedStateVector(secret_index, indexes, 1)){
#endif
    // TODO use key identifier to search the same key
    TEST_SIGNAL_HIGH;
    tree_node * p_curr_key = getKeyNode(secret_index, perm_code, perm_code_size, indexes[1], forceUpdate);
    TEST_SIGNAL_LOW;
    if(verifyMAC(p_curr_key, msg+*p_header_size, p_msg_size, implementation->MACsize)){
      TEST_SIGNAL_HIGH;
      // TODO optimization, use descriptor_id security as last tree node
      copyTreeNode(&(tmpDescriptorSecurity.key), p_curr_key);
    //TODO no signal
      TEST_SIGNAL_LOW;
      if(SUCC == updateSecurityDescriptor(descriptor_id, &tmpDescriptorSecurity, DESCRIPTOR_SECURITY_SIZE)){
        TEST_SIGNAL_HIGH;
        return descriptor_id;
      } else{
      PRINT("[ERROR] Failed to update security descriptor_id!\n"); 
      }
    }else{
      TEST_SIGNAL_HIGH;
  PRINT("=======Verify MAC %u=========\n", descriptor_id);
  printBlock("key", p_curr_key->block, p_curr_key->size);
  printBlock("msg", msg+*p_header_size, *p_msg_size);
  printBlock("getMac", msg+*p_msg_size, implementation->MACsize);
  //printBlock("sollMac", tmpMAC, size);
  PRINT("============================\n");
      PRINT("[ERROR] Failed to verify MAC!\n"); 
    }
#ifdef SECURITY_LAYER_UPDATE_STATE
  }else{
    PRINT("[ERROR] Failed to update state!\n");
  }
#endif

  return NO_DESCRIPTOR;
}
/*!
 * \}
 */

