/*-
 * applicationsend.c - Send ommand appliction for authorization server 
 *
 * Copyright 2015 Wenwen Chen
 */

/*!
 * \addtogroup    as_application 
 * \{
 *
 * \file
 * \brief       Implementation of the authorization server send command applications 
 *
 * \author      Wenwen Chen 
 */
#include "applicationsend.h"
#include "securitydescriptor.h"
#include "securitylayerhandler.h"
#include "addr_descriptor.h"
#include "tree.h"

unsigned int selected_addr_id = 0;
unsigned int selected_security_id = 0;

/*!
 * \brief Handle send message cmd to the currently chosen address
 *
 * \param req      Rest of request message after application name
 * \param req_size Size of this message
 * \param p_session pointer to the corresponding application session
 *
 * \return Response message size
 */
unsigned int handleSendTo(unsigned char* req, unsigned int req_size, application_session * p_session);
const application sendtoapplication = {
  .name = "s:",
  .name_size = 2,
  .usage = "%s\t<message>\tSend the given message to the selected address without security issues",
  .required_right = NO_RIGHT,
  .func = handleSendTo
};

unsigned int handleSendTo(unsigned char* req, unsigned int req_size, application_session * p_session){
  if(req_size < 0 || p_session == NULL){
    return 0;
  }
  if(!isActiveAddrDescriptor(selected_addr_id)){
    PRINT("%s Address descriptor %u does not exist or is not active\n", ERROR_MESSAGE, selected_addr_id);
    return 0;
  }
  p_session->addr_descriptor_id = selected_addr_id;
  p_session->security_descriptor_id = NO_DESCRIPTOR;

  memcpy(p_session->message, req, req_size);
  p_session->message_size = req_size;
  return p_session->message_size;
}

/*!
 * \brief Handle send message command with currently chosen security issues to the currently chosen address
 *
 * \param req      Rest of request message after application name
 * \param req_size Size of this message
 * \param p_session pointer to the corresponding application session
 *
 * \return Response message size
 */
unsigned int handleSecureSendTo(unsigned char* req, unsigned int req_size, application_session * p_session);
const application securesendtoapplication = {
  .name = "ss:",
  .name_size = 3,
  .usage = "%s\t<message>\tSend the given message to the selected address with security issues",
  .required_right = NO_RIGHT,
  .func = handleSecureSendTo
};

unsigned int handleSecureSendTo(unsigned char* req, unsigned int req_size, application_session * p_session){
  if(req_size < 0 || p_session == NULL){
    return 0;
  }
  if(!isActiveAddrDescriptor(selected_addr_id)){
    PRINT("%s Address descriptor %u does not exist or is not active\n", ERROR_MESSAGE, selected_addr_id);
    return 0;
  }
  if(!isActiveSecurityDescriptor(selected_security_id)){
    PRINT("%s Security descriptor %u does not exist or is not active\n", ERROR_MESSAGE, selected_security_id);
    return 0;
  }
  p_session->addr_descriptor_id = selected_addr_id;
  p_session->security_descriptor_id = selected_security_id;

  TREE_STATE_TYPE key_id = getKeyIndex(p_session->security_descriptor_id);
  setKeyIndex(p_session->security_descriptor_id, key_id+1);
  key_id = getKeyIndex(p_session->security_descriptor_id);
  /* Calculate key */
  unsigned int depth = 2;
  tree_edge * edges = getEdges(depth);
  edges[0].func = edgeFunc;
  edges[0].params = getPermCode(p_session->security_descriptor_id, &(edges[0].params_size));

  edges[1].func = edgeFunc;
  edges[1].params = (unsigned char *)(&key_id);
  edges[1].params_size = TREE_STATE_SIZE;
  tree_node * p_key_node = NULL;

  p_key_node = fillNodes(getPathFromRoot(depth), edges, depth+1, 1);
  printBlock("Key", p_key_node->block, p_key_node->size);

  if(SUCC!=updateSecurityWithKey(p_session->security_descriptor_id, p_key_node)){
    updatePredefSecurityWithKey(p_session->security_descriptor_id, p_key_node);
  }
  memcpy(p_session->message, req, req_size);
  p_session->message_size = req_size;
  return p_session->message_size;
}

/*!
 * \}
 */
