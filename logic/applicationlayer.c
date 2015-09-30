/*-
 * applicationlayerhandler.c - Application layer handler 
 *
 * Copyright 2005 Wenwen Chen
*/

/*!
 * \addtogroup    handler
 * \{
 *
 * \file
 * \brief       Implementation of the application layer functions
 *
 * \author      Wenwen Chen 
 */
#include "applicationlayer.h"
#include "application_interface.h"


unsigned int handleApplicationLayer(unsigned char * payload, unsigned int payload_size, unsigned int next_layer_descriptor, unsigned int addr_descriptor){
  if(payload_size < 1){
    return NO_SESSION;
  }
  unsigned int session_id = createApplicationSession(next_layer_descriptor, addr_descriptor);
  if(NO_SESSION != session_id){
#if(UI_APPLICATION_COUNT>0)
    if(handleApplication(payload, payload_size, session_id, msg_application) > 0){
#else
    if(handleApplication(payload, payload_size, session_id) > 0){
#endif
      return session_id;
    }
  }
  return NO_SESSION;
}

unsigned char * generateApplicationLayer(unsigned int *session, unsigned int *p_size, unsigned int *p_next_layer_descritpor){
  for(int i=0; i<APPLICATION_SESSIONS_LEN; i++){
    if((*p_size = application_sessions[i].message_size)>0){
      *session = i;
      if(p_next_layer_descritpor != NULL)
        *p_next_layer_descritpor = application_sessions[i].next_layer_descriptor;
      return application_sessions[i].message;   
    }
  }
  return NULL;
  
}
/*!
 * \}
 */
