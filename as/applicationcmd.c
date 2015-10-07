/*-
 * applicationcmd.c - Command appliction for authorization server 
 *
 * Copyright 2015 Wenwen Chen
 */

/*!
 * \addtogroup    as_application 
 * \{
 *
 * \file
 * \brief       Implementation of the authorization server command applications 
 *
 * \author      Wenwen Chen 
 */
#include "applicationcmd.h"
#include "securitydescriptor.h"
#include "securitylayerhandler.h"
#include "tree.h"
#include "bitmap.h"


//TODO add set root cmd
unsigned int handleKey(unsigned char* req, unsigned int req_size, application_session * p_session){
  if(req_size < 0 || p_session == NULL){
    return 0;
  }
  unsigned int key_index;
  if(SSCAN((const char*)req, "%u", &key_index)==1){
    /* Use the first active secret */
    unsigned int secret_index = getFirstSetBit();
    if(NO_BIT != secret_index){
      TREE_STATE_TYPE perm_index = getExpectedState(secret_index,0);

      setPermIndex(p_session->security_descriptor_id, perm_index);
      setSecretIndex(p_session->security_descriptor_id, secret_index);

      /* Calculate key */
      unsigned int depth = 2;
      tree_edge * edges = getEdges(depth);
      edges[0].func = edgeFunc;
      edges[0].params = getPermCode(p_session->security_descriptor_id, &(edges[0].params_size));

      edges[1].func = edgeFunc;
      edges[1].params = (unsigned char *)(&key_index);
      edges[1].params_size = TREE_STATE_SIZE;
      tree_node * p_key_node = NULL;

      if(p_session->addr_descriptor_id < ADDR_DESCRIPTORS_LEN){
        p_key_node = fillNodes(getPathFromCachedNodes(depth, p_session->addr_descriptor_id), edges, depth+1, 1);
      }else{
        p_key_node = fillNodes(getPathFromRoot(depth), edges, depth+1, 1);
      }
      // add security layer descriptor
      if(SUCC != updateSecurityWithKey(p_session->security_descriptor_id, p_key_node)){
        // TODO fix
        //p_session->security_descriptor_id = addSecurityDescriptor(p_key_node, NO_RIGHT, DEFAULT_PROTOCOL_TYPE);
      }

      setKeyIndex(p_session->security_descriptor_id, key_index);
      // FIXME necessary? or error msg
      if(p_session->addr_descriptor_id == NO_DESCRIPTOR){
        p_session->addr_descriptor_id = PREDEF_RS_ADDR;
      }
      memcpy(p_session->message, &"test", 4);  
      p_session->message_size = 4;
      return 4;
    }else{
      SPRINT(p_session->message, "%s No Active Secret\n", ERROR_MESSAGE);
      p_session->message_size = strlen((const char*)p_session->message);
    }
    return p_session->message_size;
  }
  SPRINT(p_session->message, "%s %s%s\n", USAGE_MESSAGE, keyapplication.name, keyapplication.usage);
  p_session->message_size = strlen((const char*)p_session->message);
  return p_session->message_size;
}

const application keyapplication = {
  .name = "key:",
  .name_size = 4,
  .usage = "%u",
  .required_right = NO_RIGHT,
  .func = handleKey
};


/*!
 * \}
 */
