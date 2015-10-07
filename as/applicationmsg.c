/*-
 * applicationmsg.c - Message appliction for authorization server 
 *
 * Copyright 2015 Wenwen Chen
*/

/*!
 * \addtogroup    as_application 
 * \{
 *
 * \file
 * \brief       Implementation of the authorization server message applications 
 *
 * \author      Wenwen Chen 
 */
#include "applicationmsg.h"
#include "securitydescriptor.h"
#include "securitylayerhandler.h"
#include "tree.h"
#include "treestate.h"
#include "bitmap.h"

const application revapplication = {
  .name = "rev:",
  .name_size = 4,
  .required_right = NO_RIGHT,
  .func = handleRevocation
};

  unsigned int handleRevocation(unsigned char* req, unsigned int req_size, application_session * p_session){
    if(req_size < 0 || p_session == NULL){
      return 0;
    }
    unsigned int secret_index;
    if(SSCAN((const char*)req, "%u", &secret_index)==1){
      // add security layer descriptor
      p_session->next_layer_descriptor = PREDEF_NO_SECURITY_DESCRIPTOR;
      if(SUCC == setBit(secret_index)){
        SPRINT((char *)(p_session->message), "Rev %d. S", secret_index);
      }else{
        SPRINT((char *)(p_session->message), "Error to Rev %d. S!", secret_index);
      }
      p_session->message_size = strlen((const char *)(p_session->message));
      return p_session->message_size;
    }
    return 0;
  }

const application permreqapplication = {
  .name = "perm:",
  .name_size = 5,
  .required_right = NO_RIGHT,
  .func = handlePermReq
};

  unsigned int handlePermReq(unsigned char* req, unsigned int req_size, application_session * p_session){
    if(req_size < 0 || p_session == NULL){
      return 0;
    }
    unsigned int perm;
    if(SSCAN((const char*)req, "%u", &perm)==1){
      /* add security layer descriptor */
      p_session->next_layer_descriptor = PREDEF_NO_SECURITY_DESCRIPTOR;
      /* Use the first active secret */
      unsigned int secret_index = getFirstNotSetBit();
      if(NO_BIT != secret_index){
        TREE_STATE_TYPE perm_index = getExpectedState(secret_index,0);
        incExpectedState(secret_index, 0, 0);
        setBit(secret_index);
        setPermIndex(p_session->next_layer_descriptor, perm_index);
        setPerm(p_session->next_layer_descriptor, (RIGHT_TYPE)perm);
        setSecretIndex(p_session->next_layer_descriptor, secret_index);
        memcpy(p_session->message, "permcode:", 9);  
        p_session->message_size = 9+generatePermCode(p_session->next_layer_descriptor, p_session->message+9, MAX_APPLICATION_MESSAGE_SIZE-9);
        /* Calculate Secret */
        unsigned int level = 2;
        tree_edge * edges = getEdges(level - 1);
        edges[0].func = edgeFunc;
        edges[0].params = getPermCode(p_session->next_layer_descriptor, &(edges[0].params_size));
        tree_node * p_key_node = NULL;

        p_key_node = fillNodes(getPathFromRoot(level), edges, level, 1);
        p_session->message[(p_session->message_size)++] = ':';
        memcpy(p_session->message+p_session->message_size, &(p_key_node->block) , p_key_node->size);  
        p_session->message_size += p_key_node->size;

      } else{
        char * error_msg = "[ERROR] No inactive Secret";
        memcpy(p_session->message, error_msg, strlen(error_msg));  
        p_session->message_size = strlen(error_msg);
      }

      return p_session->message_size;
    }
    return 0;
  }
/*!
 * \}
 */
