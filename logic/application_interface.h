#ifndef __APPLICATION_INTERFACE_H__
#define __APPLICATION_INTERFACE_H__
// TODO rename application_session
#include <string.h>
#include "securitylayer.h"

#ifdef  __cplusplus
extern "C" {
#endif

#define NO_APPLICATION 0xff
#define NO_SESSION 0xff

  typedef struct application_session{
    unsigned int application_id;
    unsigned int next_layer_descriptor;
    unsigned int addr_descriptor;
    unsigned char message[MAX_APPLICATION_MESSAGE_SIZE];
    unsigned int message_size;
  }application_session;

  extern application_session application_sessions[APPLICATION_SESSIONS_LEN];

  inline void(__attribute__((always_inline))initApplicationSession)(void){
    for(int i=0; i<APPLICATION_SESSIONS_LEN; i++){
      application_sessions[i].application_id = NO_APPLICATION;
      application_sessions[i].next_layer_descriptor = NO_DESCRIPTOR;
      application_sessions[i].next_layer_descriptor = NO_DESCRIPTOR;
    }
  }

  inline unsigned int(__attribute__((always_inline))createApplicationSession)(unsigned int next_layer_descriptor, unsigned int addr_descriptor){
    for(int i=0; i<APPLICATION_SESSIONS_LEN; i++){
      if(application_sessions[i].application_id==NO_APPLICATION){
        application_sessions[i].next_layer_descriptor = next_layer_descriptor;
        application_sessions[i].addr_descriptor = addr_descriptor;
        return i;
      }
    }

    return NO_SESSION;
  }

  inline application_session *  (__attribute__((always_inline))getApplicationSession)(unsigned int session_index){
    if(session_index < APPLICATION_SESSIONS_LEN){
      return application_sessions+session_index;
    }
    return NULL;
  }

  inline void(__attribute__((always_inline))updateApplicationSession_Application)(unsigned int session_id, unsigned int application_id){
    if(session_id < APPLICATION_SESSIONS_LEN){
      application_sessions[session_id].application_id = application_id;
    }
  }

  inline unsigned char* (__attribute__((always_inline))addApplicationSession)(unsigned int message_size, unsigned int next_layer_descriptor, unsigned int addr_descriptor){
    if(message_size > MAX_APPLICATION_MESSAGE_SIZE){
      return NULL;
    }
    unsigned int session_id = createApplicationSession(next_layer_descriptor, addr_descriptor);
    if(NO_SESSION != session_id){
      getApplicationSession(session_id)->message_size = message_size;
      return getApplicationSession(session_id)->message;
    }
    return NULL;
  }

  inline void  (__attribute__((always_inline))clearApplicationSession)(unsigned int session_index){
    if(session_index < APPLICATION_SESSIONS_LEN){
      application_sessions[session_index].message_size = 0;
      application_sessions[session_index].application_id = NO_APPLICATION;
      application_sessions[session_index].next_layer_descriptor = NO_DESCRIPTOR;
      application_sessions[session_index].addr_descriptor = NO_DESCRIPTOR;
    }
  }

  typedef struct application{
    unsigned char name[MAX_APPLICATION_NAME_SIZE];
    unsigned int name_size;
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
        if((getApplicationSession(session_id)->message_size = applications[i]->func(req+applications[i]->name_size, req_size-applications[i]->name_size, getApplicationSession(session_id)))>0){
          updateApplicationSession_Application(session_id, i);
          return getApplicationSession(session_id)->message_size;
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

#endif /* __APPLICATION_INTERFACE_H__ */

