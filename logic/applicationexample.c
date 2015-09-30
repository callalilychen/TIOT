/*-
 * applicationexample.c - Example applications 
 *
 * Copyright 2005 Wenwen Chen
*/

/*!
 * \addtogroup    applicationss
 * \{
 *
 * \file
 * \brief       Implementation of the example applications
 *
 * \author      Wenwen Chen 
 */

#include "applicationexample.h"

const application exampleapplication = {
  .name = "test",
  .name_size = 4,
  .func = handleTest
};

  unsigned int handleTest(unsigned char* req, unsigned int req_size, application_session * p_session){
    p_session->message[0] = 'O';
    p_session->message[1] = 'K';
    return 2;
  }
  
const application ackapplication = {
  .name = "ack",
  .name_size = 3,
  .func = handleAckReq
};
  unsigned int handleAckReq(unsigned char* req, unsigned int req_size, application_session * p_session){
    memcpy(p_session->message, &"ACK", 3);
    return 3;
  }

const application repapplication = {
  .name = "req",
  .name_size = 3,
  .func = handleRep
};
  unsigned int handleRep(unsigned char* req, unsigned int req_size, application_session * p_session){
    if(MAX_APPLICATION_MESSAGE_SIZE < req_size){
      memcpy(p_session->message, req, MAX_APPLICATION_MESSAGE_SIZE);
      return MAX_APPLICATION_MESSAGE_SIZE;
    }
    memcpy(p_session->message, req, req_size);
    return req_size;
  }
/*!
 * \}
 */
