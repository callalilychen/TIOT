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

//TODO descriptors?
uint8_t currVersion = 0;
static tree_node * currKeyNode = NULL;
unsigned char currMAC[HASH_SIZE] = {0};

void resetKey(void){
  currKeyNode = NULL;
}

// TODO multi descriptors
unsigned int createSecurityDescriptor(uint8_t version, tree_node * p_key){
  currVersion = version;
  currKeyNode = p_key;
  return 0;
}

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
  unsigned int size = 0;
  *p_msg_size -= mac_size;
  hmac(&sha_construction, key->block, key->size, msg, *p_msg_size, currMAC, &size);

  printBlock("MAC", currMAC, size);
  
  return memcmp(currMAC, msg+*p_msg_size, (unsigned int)mac_size);
}


unsigned int handleSecurityLayer(unsigned char *msg, unsigned int * p_msg_size, unsigned int *p_header_size){
  currVersion = VERSION_BITS(msg[0]);
    if(*p_msg_size < MAC_LEN + 1 || currVersion>MAX_VERSION){
    return NO_DESCRIPTOR;
  }
  const securityLayerImplementation * implementation = implementations[currVersion];
  if(NULL == implementation){
    return NO_DESCRIPTOR;
  }
  if(NULL == (implementation->parseHeader(msg, p_msg_size, p_header_size))){
    return NO_DESCRIPTOR;
  }

  unsigned int perm_code_size = 0;
  unsigned char* perm_code = implementation->getPermCode(&perm_code_size);
  STATE_TYPE secret_index = implementation->getSecretIndex();
  STATE_TYPE indexes [2] = {implementation->getPermIndex(),  implementation->getKeyIndex()};
  unsigned int forceUpdate = 1;
  if(SUCC == setStates(secret_index, indexes, 1, &forceUpdate)){
    if(0 == verifyMAC((currKeyNode = getKeyNode(secret_index, perm_code, perm_code_size, indexes[1], forceUpdate)), msg+*p_header_size, p_msg_size, implementation->MACsize)){
    return 0;
    }
  }

  return NO_DESCRIPTOR;
}


unsigned int generateSecurityLayerHeader(unsigned int security_descriptor, unsigned char *buf, unsigned int max_buf_size){
  //TODO
  if(security_descriptor!=0 || currVersion>MAX_VERSION){
    return 0;
  }
  return implementations[currVersion]->setHeader(buf, max_buf_size);
}

unsigned int generateSecurityLayerMAC(unsigned int security_descriptor, unsigned char *payload, unsigned int payload_size, unsigned char *buf, unsigned int max_buf_size){
  // TODO 
  if(security_descriptor!=0 || max_buf_size < implementations[currVersion]->MACsize){
    return 0;
  }
  unsigned int mac_size = 0;
  hmac(&sha_construction, currKeyNode->block, currKeyNode->size, payload, payload_size, currMAC, &mac_size);
  mac_size = implementations[currVersion]->MACsize;
  memcpy(buf, currMAC, mac_size);
  printBlock("MAC", currMAC, mac_size);
  return mac_size;
}

