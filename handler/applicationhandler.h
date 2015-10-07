/*-
 * applicationhandler.h - Application handler 
 *
 * Copyright 2015 Wenwen Chen
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
#include <string.h>
#include "applicationsession.h" 
#include "interface.h"
#ifdef  __cplusplus
extern "C" {
#endif
  /*! 
   * \brief Structure for defination an application
   *
   * \note The name of each application should be unique
   */
  typedef struct application{
    unsigned char name[MAX_APPLICATION_NAME_SIZE];
    unsigned int name_size;
    RIGHT_TYPE required_right;
    unsigned int (*func)(unsigned char*, unsigned int, application_session *);
  }application;

  /*! 
   * \brief List of supported application, which can be required using a message
   *
   *        The id of applications in the list is the index of each application
   *
   * \note The name of each application should be unique
   */
  extern const application * msg_applications[MSG_APPLICATION_COUNT];
#if(UI_APPLICATION_COUNT>0)
  /*! 
   * \brief List of supported application, which can be required via user interface
   *
   *        The id of applications in the list is MSG_APPLICATION_COUNT + the index of each application
   *
   * \note  In order to make the application id unique, the type of applications is also used to construct a application id
   */
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
        unsigned int hasRight = checkRight(getDescriptorRight(p_session -> security_descriptor_id), applications[i]->required_right);
#if(UI_APPLICATION_COUNT>0)
        if(application_type == ui_application){
          i+= MSG_APPLICATION_COUNT;
        }
#ifdef ADMIN_PASSWORD_HASH 
        if(!hasRight && application_type == ui_application){
          hasRight = (ADMIN_RIGHT == askForAdminRight());
        }
#endif
#endif
        if (hasRight && (p_session->message_size = applications[i]->func(req+applications[i]->name_size, req_size-applications[i]->name_size, p_session))>0){
          updateApplicationSession_Application(session_id, i);
          return p_session->message_size;
        }
        clearApplicationSession(session_id);
        return 0;
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
