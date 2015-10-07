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
      // add security layer descriptor
      p_session->next_layer_descriptor = PREDEF_NO_SECURITY_DESCRIPTOR;
      if(SUCC == invalidStateVector(secret_index)){
        SPRINT((char *)(p_session->message), "Rev %d. S", secret_index);
      }else{
        SPRINT((char *)(p_session->message), "Error to Rev %d. S!", secret_index);
      }
      p_session->message_size = strlen((const char *)(p_session->message));
      return p_session->message_size;
    }
    return 0;
  }


/*!
 * \}
 */
