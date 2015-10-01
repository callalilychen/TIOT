/*-
 * applicationhandler.h - Application handler 
 *
 * Copyright 2005 Wenwen Chen
*/

/*!
 * \addtogroup  handler
 * \{
 *
 * \file
 * \brief       Header definitions for the application session functions
 *
 * \author      Wenwen Chen 
 */
#ifndef __APPLICATION_HANDLER_H__
#define __APPLICATION_HANDLER_H__
#include "string.h"
#include "applicationsession.h" 
#ifdef  __cplusplus
extern "C" {
#endif
  typedef struct application{
    unsigned char name[MAX_APPLICATION_NAME_SIZE];
    unsigned int name_size;
    RIGHT_TYPE required_right;
    unsigned int (*func)(unsigned char*, unsigned int, application_session *);
  }application;

  extern const application * msg_applications[MSG_APPLICATION_COUNT];
#if(UI_APPLICATION_COUNT>0)
  extern const application * ui_applications[UI_APPLICATION_COUNT];
  
  enum{msg_application = 0, ui_application};
#else
#define applications msg_applications
#define applications_len MSG_APPLICATION_COUNT
#endif


#if(UI_APPLICATION_COUNT>0)

  inline unsigned int (__attribute__((always_inline))handleApplication)(unsigned char* req, unsigned int req_size, unsigned int session_id, int application_type){
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
  inline unsigned int (__attribute__((always_inline))handleApplication)(unsigned char* req, unsigned int req_size, unsigned int session_id){
#endif
    for(int i = 0; i < applications_len; i++){
      if(req_size >= applications[i]->name_size && memcmp(applications[i]->name, req, applications[i]->name_size)==0){
        application_session *p_session = getApplicationSession(session_id);
        if(checkRight(p_session -> has_right, applications[i]->required_right) && (p_session->message_size = applications[i]->func(req+applications[i]->name_size, req_size-applications[i]->name_size, p_session))>0){
          updateApplicationSession_Application(session_id, i);
          return p_session->message_size;
        }else{
          clearApplicationSession(session_id);
        }
      } 
    }
    return 0;
  }
  
#ifdef  __cplusplus
}
#endif /* __cplusplus */

#endif /* __APPLICATION_HANDLER_H__ */
/*!
 * \}
 */
