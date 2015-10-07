/*-
 * applicationsession.h - Application session 
 *
 * Copyright 2015 Wenwen Chen
*/

/*!
 * \defgroup    application Application
 * \{
 *
 * \file
 * \brief       Header definitions for the application session functions
 *
 * \author      Wenwen Chen 
 */
#ifndef __APPLICATION_SESSION_H__
#define __APPLICATION_SESSION_H__
#include "securityhandler.h"
#include "securitydescriptor.h"
#include "treeconfig.h"
#ifdef  __cplusplus
extern "C" {
#endif

/*!
 * \brief Macro as indicator when no session exists 
 */
#define NO_SESSION 0xff
#if NO_SESSION < APPLICATION_SESSIONS_LEN
#error "NO_SESSION should be not smaller then APPLICATION_SESSIONS_LEN, change NO_SESSION in applicationsession.h"
#endif

/*!
 * \brief Macro as indicator when no application is defined 
 */
#define NO_APPLICATION 0xff
#if NO_APPLICATION < MSG_APPLICATION_COUNT || NO_APPLICATION < UI_APPLICATION_COUNT
#error "NO_APPLICATION should be not smaller then MSG_APPLICATION_COUNT and UI_APPLICATION_COUNT, change NO_APPLICATION in applicationsession.h"
#endif
  typedef struct application_session{
    unsigned int application_id;
    RIGHT_TYPE has_right;
    // TODO rename
    unsigned int next_layer_descriptor;
    unsigned int addr_descriptor;
    unsigned char message[MAX_APPLICATION_MESSAGE_SIZE];
    unsigned int message_size;
  }application_session;

  extern application_session application_sessions[APPLICATION_SESSIONS_LEN];

  inline void(__attribute__((always_inline))initApplicationSession)(void){
    for(int i=0; i<APPLICATION_SESSIONS_LEN; i++){
      application_sessions[i].application_id = NO_APPLICATION;
      application_sessions[i].has_right = NO_RIGHT;
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

  
#ifdef  __cplusplus
}
#endif /* __cplusplus */

#endif /* __APPLICATION_SESSION_H__ */
/*!
 * \}
 */
