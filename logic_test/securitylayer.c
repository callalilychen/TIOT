#include "securitylayer.h"
#include "securitylayerV0.h"
#include "securitylayerV1.h"

const static securityLayerImplementation securityLayerImplementationV0 = {
  .parseHeader = parseHeaderV0,
  .getHeader = getHeaderV0,
  .getPermCode = getPermCodeV0,
  .getSecretIndex = getSecretIndexV0,
  .getPermIndex = getPermIndexV0,
  .getKeyIndex = getKeyIndexV0,
  .setHeader = setHeaderV0,
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

static const securityLayerImplementation* implementations[MAX_VERSION + 1] = {&securityLayerImplementationV0, NULL, NULL, NULL};

//TODO session?
static tree_node * currKeyNode = NULL;

void resetKey(void){
  currKeyNode = NULL;
}

static inline tree_node * getKeyNode(STATE_INDEX_TYPE secret_index, unsigned char * perm_code, unsigned int perm_code_size, STATE_INDEX_TYPE key_index, unsigned int flag){
  
  tree_edge * edges = getEdges(3);

  edges[0].func = edgeFunc;
  edges[1].func = edgeFunc;

  edges[0].params = perm_code;
  edges[0].params_size = perm_code_size;


  edges[1].params = (unsigned char *)(&key_index);
  edges[1].params_size = STATE_INDEX_SIZE;

  if(flag){
    return fillNodes(getPathFromRoot(3), edges, 3, flag);
  }else{
    return fillNodes(getPathFromRootWithCachedNodes(3, secret_index), edges, 3, flag);
  }
}

//FIXME
static void printBlock(char* name, unsigned char* block, unsigned int block_len){
  PRINT("%s:\n", name);
  for (int i=0; i< block_len; i++){
    PRINT("%x|", block[i]);
  }
  PRINT("\n");
}

static inline int verifyMAC(tree_node * key, unsigned char * msg, unsigned int *p_msg_size, unsigned int mac_size){
  unsigned char mac[HASH_SIZE] = {0};
  unsigned int size = 0;
  *p_msg_size -= mac_size;
  hmac(&sha_construction, key->block, key->size, msg, *p_msg_size, mac, &size);

  printBlock("MAC", mac, size);
  
  return memcmp(&mac, msg+*p_msg_size, (unsigned int)mac_size);
}


int handleSecurityLayer(uint8_t version, unsigned char *msg, unsigned int * p_msg_size, unsigned int *p_header_size){
    if(*p_msg_size < MAC_LEN + 1 || version>MAX_VERSION){
    return FAIL;
  }
  const securityLayerImplementation * implementation = implementations[version];
  if(NULL == implementation){
    return FAIL;
  }
  if(NULL == (implementation->parseHeader(msg, p_msg_size, p_header_size))){
    return FAIL;
  }

  unsigned int perm_code_size = 0;
  unsigned char* perm_code = implementation->getPermCode(&perm_code_size);
  STATE_INDEX_TYPE secret_index = implementation->getSecretIndex();
  STATE_INDEX_TYPE perm_index = implementation->getPermIndex();
  STATE_INDEX_TYPE key_index = implementation->getKeyIndex();
  unsigned int forceUpdate = 1;
  if(SUCC == setState(secret_index, perm_index, key_index, &forceUpdate)){
    if(0 == verifyMAC((currKeyNode = getKeyNode(secret_index, perm_code, perm_code_size, key_index, forceUpdate)), msg+*p_header_size, p_msg_size, implementation->MACsize)){
    return SUCC;
    }
  }

  return FAIL;
}


int handleSecurityLayerAfterPayload(uint8_t version, unsigned char *payload, unsigned int payload_size, unsigned char *buf, unsigned int *buf_size){
  hmac(&sha_construction, currKeyNode->block, currKeyNode->size, payload, payload_size, buf, (unsigned int *)buf_size);
  return SUCC;
}

