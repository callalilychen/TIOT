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
    if(SSCAN((const char*)req, "%u", &key_index)==1){

      // Calculate key
      tree_edge * edges = getEdges(1);
      edges[0].func = edgeFunc;

      edges[0].params = (unsigned char *)(&key_index);
      edges[0].params_size = STATE_SIZE;
      // add security layer descriptor
      if(p_session->next_layer_descriptor == NO_DESCRIPTOR)
      {
        if(p_session->addr_descriptor < ADDR_DESCRIPTORS_LEN) 
        {
          p_session->next_layer_descriptor = addSecurityDescriptor(DEFAULT_VERSION,fillNodes(getPathFromCachedNodes(2, p_session->addr_descriptor), edges, 2, 1));
        }else{
          p_session->next_layer_descriptor = addSecurityDescriptor(DEFAULT_VERSION,fillNodes(getPathFromRoot(2), edges, 2, 1));
        }
        setKeyIndex(p_session->next_layer_descriptor, key_index);
      }
      if(p_session->addr_descriptor == NO_DESCRIPTOR)
        p_session->addr_descriptor = PREDEF_RS_ADDR;
      memcpy(p_session->message, &"test", 4);  
      p_session->message_size = 4;
      return 4;
    }
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
    // TODO do sth. with secret_index
    if(SSCAN((const char*)req, "%u", &secret_index)==1){
      // add security layer descriptor
      p_session->next_layer_descriptor = PREDEF_NO_SECURITY_DESCRIPTOR;
      p_session->addr_descriptor = PREDEF_AS_ADDR;
      memcpy(p_session->message, revapplication.name, revapplication.name_size);  
      memcpy(p_session->message + revapplication.name_size, req, req_size);  
      p_session->message_size = revapplication.name_size + req_size;
      return p_session->message_size;
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
    unsigned int key_index;
    if(SSCAN((const char*)req, "%u", &key_index)==1){

      // add security layer descriptor
      p_session->next_layer_descriptor = PREDEF_NO_SECURITY_DESCRIPTOR;
      p_session->addr_descriptor = PREDEF_AS_ADDR;
      memcpy(p_session->message, permreqapplication.name, permreqapplication.name_size);  
      memcpy(p_session->message + permreqapplication.name_size, req, req_size);  
      p_session->message_size = permreqapplication.name_size + req_size;
      return p_session->message_size;
    }
    return 0;
  }
