/*-
 * applicationexample.c - Example applications 
 *
 * Copyright 2015 Wenwen Chen
 */

/*!
 * \addtogroup    application
 * \{
 *
 * \file
 * \brief       Implementation of the example applications
 *
 * \author      Wenwen Chen 
 */

#include "applicationexample.h"

/*!
 * \brief Handle test request and response 'OK' back 
 * 
 * \param req       Rest of request message after application name
 * \param req_size  Size of this message
 * \param p_session pointer to the corresponding application session
 *
 * \return          Response message size, which is 4
 */
unsigned int handleTest(unsigned char* req, unsigned int req_size, application_session * p_session){
  memcpy(p_session->message, &"[OK]", 4);
  p_session->message[4] = '\0';
  return 4;
}

const application exampleapplication = {
  .name = "test",
  .name_size = 4,
#if(MAX_APPLICATION_USAGE_SIZE>0)
  .usage = "\t\tTest request, [OK] will be responed",
#endif
  .required_right = NO_RIGHT,
  .func = handleTest
};

/*!
 * \brief Handle ack request and response '[ACK]' back 
 * 
 * \param req       Rest of request message after application name
 * \param req_size  Size of this message
 * \param p_session pointer to the corresponding application session
 *
 * \return          Response message size, which is 5
 */
unsigned int handleAckReq(unsigned char* req, unsigned int req_size, application_session * p_session){
  memcpy(p_session->message, &"[ACK]", 5);
  p_session->message[5] = '\0';
  return 5;
}

const application ackapplication = {
  .name = "ack",
  .name_size = 3,
#if(MAX_APPLICATION_USAGE_SIZE>0)
  .usage = "\t\tAck request, [ACK] will be responsed",
#endif
  .required_right = NO_RIGHT,
  .func = handleAckReq
};

/*!
 * \}
 */
