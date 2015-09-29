#include "applicationcmd.h"

const application keyapplication = {
  .name = "key:",
  .name_size = 4,
  .func = handleKey
};

  unsigned int handleKey(unsigned char* req, unsigned int req_size, application_session * p_session){
    if(req_size < 0 || p_session == NULL){
      return 0;
    }
    unsigned int key_index;
    //if(SSCAN((const char*)req, "%u", &key_index)==1){
    //  /* Use the first active secret */
    //  unsigned int secret_index = getFirstSetBit();
    //  if(NO_BIT != secret_index){
    //    STATE_TYPE perm_index = getExpectedState(secret_index,0);
    //    
    //    // add security layer descriptor
    //    if(p_session->next_layer_descriptor == NO_DESCRIPTOR){
    //      p_session->next_layer_descriptor = createSecurityDescriptor();
    //    }
    //    
    //    setPermIndex(p_session->next_layer_descriptor, perm_index);
    //    setSecretIndex(p_session->next_layer_descriptor, secret_index);

    //    /* Calculate key */
    //    unsigned int level = 3;
    //    tree_edge * edges = getEdges(level - 1);
    //    edges[0].func = edgeFunc;
    //    edges[0].params = getPermCode(p_session->next_layer_descriptor, &(edges[0].params_size));

    //    edges[1].func = edgeFunc;
    //    edges[1].params = (unsigned char *)(&key_index);
    //    edges[1].params_size = STATE_SIZE;
    //    tree_node * key_node = NULL;

    //    if(p_session->addr_descriptor < ADDR_DESCRIPTORS_LEN){
    //      key_node = fillNodes(getPathFromCachedNodes(level, p_session->addr_descriptor), edges, level, 1);
    //    }else{
    //      key_node = fillNodes(getPathFromRoot(level), edges, level, 1);
    //    }
    //    // add security layer descriptor
    //    updateKey(p_session->next_layer_descriptor, key_node); 

    //    setKeyIndex(p_session->next_layer_descriptor, key_index);
    //    if(p_session->addr_descriptor == NO_DESCRIPTOR){
    //      p_session->addr_descriptor = PREDEF_RS_ADDR;
    //    }
    //    memcpy(p_session->message, &"test", 4);  
    //    p_session->message_size = 4;
    //    return 4;
    //  }else{
    //    memcpy(p_session->message, "Error to Key!", 14);  
    //    p_session->message_size = 14;
    //  }
    //}
    return 0;
  }

const application revapplication = {
  .name = "rev:",
  .name_size = 4,
  .func = handleRevocation
};

  unsigned int handleRevocation(unsigned char* req, unsigned int req_size, application_session * p_session){
    if(req_size < 0 || p_session == NULL){
      return 0;
    }
    unsigned int secret_index;
    if(SSCAN((const char*)req, "%u", &secret_index)==1){
    //  // add security layer descriptor
    //  p_session->next_layer_descriptor = PREDEF_NO_SECURITY_DESCRIPTOR;
    //  if(SUCC == setBit(secret_index)){
    //    SPRINT((char *)(p_session->message), "Rev %d. S", secret_index);
    //  }else{
    //    SPRINT((char *)(p_session->message), "Error to Rev %d. S!", secret_index);
    //  }
    //  p_session->message_size = strlen((const char *)(p_session->message));
    //  return p_session->message_size;
    }
    return 0;
  }

const application permreqapplication = {
  .name = "perm:",
  .name_size = 5,
  .func = handlePermReq
};

  unsigned int handlePermReq(unsigned char* req, unsigned int req_size, application_session * p_session){
    if(req_size < 0 || p_session == NULL){
      return 0;
    }
    unsigned int perm;
    //if(SSCAN((const char*)req, "%u", &perm)==1){
    //  //TODO handle perm
    //  /* add security layer descriptor */
    //  p_session->next_layer_descriptor = PREDEF_NO_SECURITY_DESCRIPTOR;
    //  /* Use the first active secret */
    //  unsigned int secret_index = getFirstNotSetBit();
    //  if(NO_BIT != secret_index){
    //    STATE_TYPE perm_index = getExpectedState(secret_index,0);
    //    incExpectedState(secret_index, 0, 0);
    //    setBit(secret_index);
    //    setPermIndex(p_session->next_layer_descriptor, perm_index);
    //    setSecretIndex(p_session->next_layer_descriptor, secret_index);
    //    memcpy(p_session->message, "permcode:", 9);  
    //    p_session->message_size = 9+generatePermCode(p_session->next_layer_descriptor, p_session->message+9, MAX_APPLICATION_MESSAGE_SIZE-9);
    //    /* Calculate Secret */
    //    unsigned int level = 2;
    //    tree_edge * edges = getEdges(level - 1);
    //    edges[0].func = edgeFunc;
    //    edges[0].params = getPermCode(p_session->next_layer_descriptor, &(edges[0].params_size));
    //    tree_node * key_node = NULL;

    //    key_node = fillNodes(getPathFromRoot(level), edges, level, 1);
    //    p_session->message[(p_session->message_size)++] = ':';
    //    memcpy(p_session->message+p_session->message_size, &(key_node->block) , key_node->size);  
    //    p_session->message_size += key_node->size;

    //  } else{
    //    memcpy(p_session->message, "Error to Perm!", 14);  
    //    p_session->message_size = 14;
    //  }

    //  return p_session->message_size;
    //}
    return 0;
  }
