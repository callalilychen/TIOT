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
//#include "treestate.h"
#include "bitmap.h"

const application revapplication = {
  .name = "rev:",
  .name_size = 4,
  .required_right = ADMIN_RIGHT,
  .func = handleRevocation
};

  unsigned int handleRevocation(unsigned char* req, unsigned int req_size, application_session * p_session){
    if(req_size < 0 || p_session == NULL){
      return 0;
    }
    unsigned int secret_index;
    if(SSCAN((const char*)req, "%u", &secret_index)==1){
      if(SUCC == invalidStateVector(secret_index)){
        memcpy(p_session->message, DONE_MESSAGE, DONE_MESSAGE_SIZE);
        p_session->message[DONE_MESSAGE_SIZE] = ' ';
        p_session->message_size = DONE_MESSAGE_SIZE+1;
      }else{
        memcpy(p_session->message, ERROR_MESSAGE, ERROR_MESSAGE_SIZE);
        p_session->message[ERROR_MESSAGE_SIZE] = ' ';
        p_session->message_size = ERROR_MESSAGE_SIZE+1;
      }
      memcpy(p_session->message + p_session->message_size, revapplication.name, revapplication.name_size);
      p_session->message_size +=  revapplication.name_size;
      return p_session->message_size;
    }
    return 0;
  }


/*!
 * \}
 */
