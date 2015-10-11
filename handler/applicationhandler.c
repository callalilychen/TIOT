/*-
 * applicationhandler.c - Application handler 
 *
 * Copyright 2015 Wenwen Chen
*/

/*!
 * \addtogroup  handler
 * \{
 *
 * \file
 * \brief       Implementation of the application session functions
 *
 * \author      Wenwen Chen 
 */
#include "applicationhandler.h"
 
unsigned int generateApplicationHeader(unsigned char* buf, unsigned int max_buf_size, int type){
  switch(type){
    case SUCC:
      if(max_buf_size > SUCC_MESSAGE_SIZE){
        memcpy(buf, SUCC_MESSAGE, SUCC_MESSAGE_SIZE);
        buf[SUCC_MESSAGE_SIZE] = ' ';
        return SUCC_MESSAGE_SIZE+1;
      }
      break;
    case DONE:
      if(max_buf_size > DONE_MESSAGE_SIZE){
        memcpy(buf, DONE_MESSAGE, DONE_MESSAGE_SIZE);
        buf[DONE_MESSAGE_SIZE] = ' ';
        return DONE_MESSAGE_SIZE+1;
      }
      break;
    case INFO:
      if(max_buf_size > INFO_MESSAGE_SIZE){
        memcpy(buf, INFO_MESSAGE, INFO_MESSAGE_SIZE);
        buf[INFO_MESSAGE_SIZE] = ' ';
        return INFO_MESSAGE_SIZE+1;
      }
      break;
    case FAIL:
    default:
      if(max_buf_size > ERROR_MESSAGE_SIZE){
        memcpy(buf, ERROR_MESSAGE, ERROR_MESSAGE_SIZE);
        buf[ERROR_MESSAGE_SIZE] = ' ';
        return ERROR_MESSAGE_SIZE+1;
      }
      break;
  }
  return 0;
}

unsigned int generateApplicationStatusResponse(const application* p_application, application_session * p_session, int type, unsigned char * status, unsigned int status_size){
  p_session->message_size = generateApplicationHeader(p_session->message, MAX_APPLICATION_MESSAGE_SIZE - p_application->name_size - status_size,  type);
    if(p_session->message_size > 0){
      memcpy(p_session->message + p_session->message_size, p_application->name, p_application->name_size);
      p_session->message_size +=  p_application->name_size;
      memcpy(p_session->message + p_session->message_size, status, status_size);
      p_session->message_size +=  status_size;
      p_session->message[p_session->message_size] = '\0';
  }
  PRINT("Response (%u): %s\n", p_session->message_size, p_session->message);
  return p_session->message_size;
}
/*!
 * \}
 */
