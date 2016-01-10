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
      //memcpy(p_session->message + p_session->message_size, p_application->name, p_application->name_size);
      //p_session->message_size +=  p_application->name_size;
      memcpy(p_session->message + p_session->message_size, status, status_size);
      p_session->message_size +=  status_size;
      p_session->message[p_session->message_size] = '\0';
  }
  VPRINT("Response (%u): %s\n", p_session->message_size, p_session->message);
  PRINT("v ");
  return p_session->message_size;
}

#if(UI_APPLICATION_COUNT>0)
   unsigned int handleApplication(unsigned char* req, unsigned int req_size, unsigned int session_id, int application_type){
    application ** applications; 
    int applications_len = 0;
    switch(application_type){
      case msg_application:
        applications = (application **)msg_applications;
        applications_len = MSG_APPLICATION_COUNT;
        break;
      case ui_application:
        applications = (application **)ui_applications;
        applications_len = UI_APPLICATION_COUNT;
        break;
      default:
        return 0;
    }
#else
     unsigned int handleApplication(unsigned char* req, unsigned int req_size, unsigned int session_id){
#endif
      for(int i = 0; i < applications_len; i++){
        if(req_size >= applications[i]->name_size && memcmp(applications[i]->name, req, applications[i]->name_size)==0){
          application_session *p_session = getApplicationSession(session_id);
          unsigned int hasRight = checkRight(getPerm(p_session -> security_descriptor_id), applications[i]->required_right);
#if(UI_APPLICATION_COUNT>0)
#ifdef ADMIN_PASSWORD_HASH 
          if(!hasRight && application_type == ui_application){
            hasRight = (ADMIN_RIGHT == askForAdminRight());
          }
#endif
#endif
          if(!hasRight){
            //p_session -> security_descriptor_id = NO_DESCRIPTOR;
            deactiveSecurityDescriptor(p_session -> security_descriptor_id);
            memcpy(p_session->message, "[ERR] NOP", 9);
            p_session->message[9] = '\0';
            p_session->message_size = strlen((const char*)(p_session->message));
            PRINT("%s\n", p_session->message);
          } else if ((p_session->message_size = applications[i]->func(req+applications[i]->name_size, req_size-applications[i]->name_size, p_session))>0){
#if(UI_APPLICATION_COUNT>0)
            if(application_type == ui_application){
              i+= MSG_APPLICATION_COUNT;
            }
#endif
          }else{
            clearApplicationSession(session_id);
            return 0;
          }
        updateApplicationSession_Application(session_id, i);
          return p_session->message_size;
        } 
      }
      return 0;
    }

/*!
 * \}
 */
