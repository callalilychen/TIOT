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

  /*!
   * \brief Session information for handlers to handle an application
   */
  typedef struct application_session{
    unsigned int application_id; /*!< Id of required application*/
    unsigned int security_descriptor_id;  /*!< Descriptor id for security issues*/
    unsigned int addr_descriptor_id;  /*!< Descriptor id for source address, who requests the application*/
    unsigned char message[MAX_APPLICATION_MESSAGE_SIZE]; /*!< Response message */
    unsigned int message_size; /*!< Message size */
  }application_session;

  /*!
   * \brief Static memory allocation for storage of application sessions
   */
  extern application_session application_sessions[APPLICATION_SESSIONS_LEN];

  /*!
   * \brief Reset all application sessions
   *
   * \return None
   */
  inline void __attribute__((always_inline))resetApplicationSession(void){
    for(int i=0; i<APPLICATION_SESSIONS_LEN; i++){
      application_sessions[i].application_id = NO_APPLICATION;
      application_sessions[i].security_descriptor_id = NO_DESCRIPTOR;
      application_sessions[i].addr_descriptor_id = NO_DESCRIPTOR;
      application_sessions[i].message_size = 0;
    }
  }

  /*!
   * \brief Create an application session and assign its security_descriptor_id and addr_descriptor_id
   *
   * \param security_descriptor_id    The given descriptor id for security issues
   * \param addr_descriptor_id        The given descriptor id for source address
   *
   * \return                          Session id of the new application session or NO_SESSION, if all reserved place for application sessions is full
   */
  inline unsigned int __attribute__((always_inline))createApplicationSession(unsigned int security_descriptor_id, unsigned int addr_descriptor_id){
    for(int i=0; i<APPLICATION_SESSIONS_LEN; i++){
      if(application_sessions[i].application_id==NO_APPLICATION){
        application_sessions[i].security_descriptor_id = security_descriptor_id;
        application_sessions[i].addr_descriptor_id = addr_descriptor_id;
        application_sessions[i].message_size = 0;
        return i;
      }
    }
    return NO_SESSION;
  }

  /*!
   * \brief Use the application session of the given id
   *      
   *        Fill the session informations to the given pointers and return a pointer to the response message of the given session id, if the session is active. That means the session contains some response message: .message_size is bigger than 0
   *
   * \param session_id    The given session id
   * \param p_size        Pointer to store the session message length
   * \param p_security_descriptor_id Pointer to the security descriptor id
   * \param p_addr_descriptor_id     Pointer to the address descriptor id
   *
   * \return              Pointer to the message of the application or NULL, if the session of the given id is not active or the given id is out of range
   */
  inline unsigned char * __attribute__((always_inline))useApplicationSession(unsigned int session_id, unsigned int * p_size, unsigned int * p_security_descriptor_id, unsigned int * p_addr_descriptor_id){
    if(session_id < APPLICATION_SESSIONS_LEN){
      unsigned int size = application_sessions[session_id].message_size;
      if(size > 0){
        if(p_size!=NULL){
          *p_size = size;
        }
        if(p_security_descriptor_id!=NULL){
          *p_security_descriptor_id = application_sessions[session_id].security_descriptor_id;
        }
        if(p_addr_descriptor_id!=NULL){
          *p_addr_descriptor_id = application_sessions[session_id].addr_descriptor_id;
        }
        return application_sessions[session_id].message;
      }
    }
    return NULL;
  }

  /*!
   * \brief Get the response message size of the application session of the given id 
   *
   * \param session_id    The given session id
   *
   * \return              Size of response message
   */
  inline unsigned int __attribute__((always_inline))getApplicationMessageSize(unsigned int session_id){
    if(session_id < APPLICATION_SESSIONS_LEN){
      return application_sessions[session_id].message_size;
    }
    return 0;
  }

  /*!
   * \brief Get a pointer to the application session of the given id 
   *
   * \param session_id    The given session id
   *
   * \return              Pointer to the session of the new application session or NULL, if the given id is out of range
   */
  inline application_session * __attribute__((always_inline))getApplicationSession(unsigned int session_id){
    if(session_id < APPLICATION_SESSIONS_LEN){
      return application_sessions+session_id;
    }
    return NULL;
  }
  
  /*!
   * \brief Update the application of the application session of the given session id
   *
   * \param session_id    The given session id
   *
   * \return              None
   */
  inline void __attribute__((always_inline))updateApplicationSession_Application (unsigned int session_id, unsigned int application_id){
    if(session_id < APPLICATION_SESSIONS_LEN){
      application_sessions[session_id].application_id = application_id;
    }
  }

  /*!
   * \brief Reset the application session of the given id
   *
   * \param session_id    The given session id
   *
   * \return              None
   */
  inline void  __attribute__((always_inline))clearApplicationSession(unsigned int session_id){
    if(session_id < APPLICATION_SESSIONS_LEN){
      application_sessions[session_id].application_id = NO_APPLICATION;
      application_sessions[session_id].security_descriptor_id = NO_DESCRIPTOR;
      application_sessions[session_id].addr_descriptor_id = NO_DESCRIPTOR;
      application_sessions[session_id].message_size = 0;
    }
  }

  
#ifdef  __cplusplus
}
#endif /* __cplusplus */

#endif /* __APPLICATION_SESSION_H__ */
/*!
 * \}
 */
