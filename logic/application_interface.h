#ifndef __APPLICATION_INTERFACE_H__
#define __APPLICATION_INTERFACE_H__

#include <string.h>
#include "securitylayer.h"

#ifdef  __cplusplus
extern "C" {
#endif

#define NO_APPLICATION 0
#define NO_SESSION 0x0
#define MAX_APPLICATION_SESSION 2
#define MAX_APPLICATION_MESSAGE_SIZE 10

  typedef struct application_session{
    unsigned int next_layer_descriptor;
    unsigned int application_id;
    unsigned char message[MAX_APPLICATION_MESSAGE_SIZE];
    unsigned int message_size;
  }application_session;

  extern application_session application_sessions[MAX_APPLICATION_SESSION];

  inline unsigned char* (__attribute__((always_inline))addApplicationSession)(unsigned int message_size, unsigned int next_layer_descriptor){
    if(message_size > MAX_APPLICATION_MESSAGE_SIZE){
      return NULL;
    }
    for(int i=0; i<MAX_APPLICATION_SESSION; i++){
      if(application_sessions[i].application_id==NO_APPLICATION){
        application_sessions[i].message_size = message_size;
        application_sessions[i].next_layer_descriptor = next_layer_descriptor;
        return application_sessions[i].message;
      }
    }

    return NULL;
  }

#define MAX_APPLICATION_COUNT 2
#define MAX_APPLICATION_NAME 4

  typedef struct application{
    unsigned char name[MAX_APPLICATION_NAME];
    unsigned int (*func)(unsigned char* , unsigned int);
  }application;

  extern const application * applications[MAX_APPLICATION_COUNT];

  inline unsigned int (__attribute__((always_inline))handleApplication)(unsigned char* req, unsigned int req_size, application_session *p_session){
    if(req_size >=3  && memcmp(&"rep", req, 3)==0){

      memcpy(p_session->message, req, (size_t)req_size);
      p_session->application_id = MAX_APPLICATION_COUNT+2; 
      return req_size;
    }
    
    for(int i = 0; i < MAX_APPLICATION_COUNT; i++){
      if(req_size <= MAX_APPLICATION_NAME && memcmp(applications[i]->name, req, req_size)==0){
        p_session->application_id = i+1; 
        return applications[i]->func(p_session->message, MAX_APPLICATION_MESSAGE_SIZE); 
      } 
    }
    return 0;
  }
  
  inline void  (__attribute__((always_inline))clearApplicationLayerSession)(unsigned int session_index){
    if(session_index == NO_SESSION){
      return;
    }
    session_index--;
    if(session_index < MAX_APPLICATION_SESSION){
      application_sessions[session_index].message_size = 0;
      application_sessions[session_index].application_id = NO_APPLICATION;
      application_sessions[session_index].next_layer_descriptor = NO_DESCRIPTOR;
    }
  }
#ifdef  __cplusplus
}
#endif /* __cplusplus */

#endif /* __APPLICATION_INTERFACE_H__ */

