/*-
 * applicationrsstate.c - Message appliction for state update on constrained resource server 
 *
 * Copyright 2015 Wenwen Chen
 */

/*!
 * \addtogroup    constrained_rs_application 
 * \{
 *
 * \file
 * \brief       Implementation of the constrained server state update applications 
 *
 * \author      Wenwen Chen 
 */
#include "applicationrsstate.h"
#include "applicationhandler.h"
#include "treestate.h"

unsigned int handleRSRevocation(unsigned char* req, unsigned int req_size, application_session * p_session);
const application rsrevapplication = {
  .name = "rev:",
  .name_size = 4,
#if(MAX_APPLICATION_USAGE_SIZE>0)
  .usage = "%u\t<id>\tRevoke a secret",
#endif
  .required_right = ADMIN_RIGHT,
  .func = handleRSRevocation
};
/*!
 * \brief Handle revocation message
 *
 * \param req      Rest of request message after application name
 * \param req_size Size of this message
 * \param p_session pointer to the corresponding application session
 *
 * \return Response message size
 */
unsigned int handleRSRevocation(unsigned char* req, unsigned int req_size, application_session * p_session){
  if(req_size < 0 || p_session == NULL){
    return 0;
  }
  unsigned int secret_index;
  if(SSCAN((const char*)req, "%u", &secret_index)==1){
    p_session->message_size = generateApplicationHeader(p_session->message, MAX_APPLICATION_MESSAGE_SIZE-req_size - rsrevapplication.name_size,invalidStateVector(secret_index));
    if(p_session->message_size > 0){
      memcpy(p_session->message + p_session->message_size, req-rsrevapplication.name_size, req_size+rsrevapplication.name_size);
      p_session->message_size +=  req_size + rsrevapplication.name_size;
    }
  }
  return p_session->message_size;
}



/*!
 * \}
 */
