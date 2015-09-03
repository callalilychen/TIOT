#include "securitylayer.h"
#include "securitylayerV0.h"
#include "securitylayerV1.h"

const static securityLayerImplementation securityLayerImplementationV0 = {
  .setHeader = setHeaderV0,
  .getSecretIndex = getSecretIndexV0,
  .getPermIndex = getPermIndexV0,
  .getKeyIndex = getKeyIndexV0,
  .setSecretIndex = setSecretIndexV0,
  .setPermIndex = setPermIndexV0,
  .setKeyIndex = setKeyIndexV0,
  .MACsize = MAC_LEN_V0,
};

const static securityLayerImplementation securityLayerImplementationV1 = {
  .setHeader = setHeaderV1,
  .getSecretIndex = getSecretIndexV1,
  .getPermIndex = getPermIndexV1,
  .getKeyIndex = getKeyIndexV1,
  .setSecretIndex = setSecretIndexV1,
  .setPermIndex = setPermIndexV1,
  .setKeyIndex = setKeyIndexV1,
  .MACsize = MAC_LEN_V1,
};

static securityLayerImplementation* implementations[MAX_VERSION + 1] = {&securityLayerImplementationV0, &securityLayerImplementationV1, NULL, NULL};

static descendant branch[2] = {0};
static  unsigned char nodes[2][HASH_BLOCK_LENGTH];

static position pos = {
  .levels = 0,
  .descendants = branch
};

static unsigned char * currKey;
static unsigned int currKeySize = 0;

static inline unsigned char * getKey(STATE_INDEX_TYPE index, STATE_INDEX_TYPE perm_index, STATE_INDEX_TYPE key_index){
  STATE_INDEX_TYPE p1[2] = {index, perm_index};
  branch[0].func = EDGE_FUNC;
  branch[0].param = (unsigned char *)p1;
  branch[0].param_len = 2*sizeof(STATE_INDEX_TYPE);
  branch[0].child = nodes[0];
  branch[0].child_len = 0;

  branch[1].func = EDGE_FUNC;
  branch[1].param = (unsigned char *)&key_index;
  branch[1].param_len = sizeof(STATE_INDEX_TYPE);
  branch[1].child = nodes[1];
  branch[1].child_len = 0;

  pos.levels = 2;

  return getNode(&pos, &currKeySize);
}

static inline int verifyMAC(unsigned char * key, size_t key_size, unsigned char * msg, size_t msg_size, uint8_t mac_size){
  unsigned char mac[HASH_BLOCK_LENGTH] = {0};
  hmac(HASH_FUNC, HASH_BLOCK_LENGTH, key, key_size, msg, msg_size, mac);
  
  return memcmp(&mac, msg+msg_size, (size_t)mac_size);
}


int handleSecurityLayer(uint8_t version, unsigned char *msg, size_t msg_size, unsigned char **p_payload, size_t *p_payload_size, unsigned char *buf, size_t *p_buf_size){
    if(msg_size < MAC_LEN + 1 || version>MAX_VERSION){
    return FAIL;
  }
  securityLayerImplementation * implementation = implementations[version];
  if(implementation == NULL){
    return FAIL;
  }
  uint8_t header_size = 0;
  if(FAIL == *(implementation->setHeader(msg, msg_size, &header_size))){
    return FAIL;
  }
  
  *p_payload = msg + header_size;
  *p_payload_size = msg_size - header_size - implementation->MACsize;

  STATE_INDEX_TYPE secret_index = implementation->getSecretIndex();
  STATE_INDEX_TYPE perm_index = implementation->getPermIndex();
  STATE_INDEX_TYPE key_index = implementation->getKeyIndex();

  if(SUCC == setState(secret_index, perm_index, key_index)){
    currKey = getKey(secret_index, perm_index, key_index);
    if(0 == verifyMAC(currKey, currKeySize, *p_payload, *p_payload_size, implementation->MACsize)){
      if()
    return SUCC;
    }
  }

  return FAIL;
}


int handleSecurityLayerAfterPayload(uint8_t version, unsigned char *payload, size_t payload_size, unsigned char *buf, size_t *buf_size){
  MAC_FUNC(HASH_FUNC, HASH_BLOCK_LENGTH, currKey, currKeySize, payload, payload_size, buf);
  *buf_size = HASH_BLOCK_LENGTH;
  return SUCC;
}

