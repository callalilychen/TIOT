/*-
 * applicationexample.h - Example applications 
 *
 * Copyright 2015 Wenwen Chen
*/

/*!
 * \addtogroup    application
 * \{
 *
 * \file
 * \brief       Header definitions for the example applications
 *
 * \author      Wenwen Chen 
 */
#ifndef __APPLICATION_EXAMPLE_H__
#define __APPLICATION_EXAMPLE_H__

#include "applicationhandler.h"

#ifdef  __cplusplus
extern "C" {
#endif

  extern const application exampleapplication;
  extern const application ackapplication;
  extern const application repapplication;
  
  unsigned int handleTest(unsigned char* , unsigned int, application_session *);
  unsigned int handleAckReq(unsigned char* , unsigned int, application_session *);
  unsigned int handleRep(unsigned char* , unsigned int, application_session *);

  inline unsigned int (__attribute__((always_inline))generateTest)(unsigned int next_layer_descriptor_id, unsigned int addr_descriptor_id){
    application_session *p_session = getApplicationSession(createApplicationSession(next_layer_descriptor_id, addr_descriptor_id));
    if(p_session==NULL){
      return 0;
    }
    memcpy(p_session->message, "TEST", 4);
    p_session->message_size = 4;
    return 4;
  }

  inline unsigned int (__attribute__((always_inline))generateAckReq)(unsigned int next_layer_descriptor_id, unsigned int addr_descriptor_id){
    application_session *p_session = getApplicationSession(createApplicationSession(next_layer_descriptor_id, addr_descriptor_id));
    if(p_session==NULL){
      return 0;
    }
    memcpy(p_session->message, "ACK", 3);
    p_session->message_size = 3;
    return 3;
  }

#ifdef  __cplusplus
}
#endif /* __cplusplus */

#endif /* __APPLICATION_EXAMPLE_H__ */
/*!
 * \}
 */
