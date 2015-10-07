/*-
 * applicationlayerhandler.c - Application layer handler 
 *
 * Copyright 2015 Wenwen Chen
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
#include "applicationlayerhandler.h"
#include "applicationhandler.h"


unsigned int handleApplicationLayer(unsigned char * message, unsigned int message_size, unsigned int security_descriptor_id, unsigned int addr_descriptor_id){
  if(message_size < 1){
    return NO_SESSION;
  }
  unsigned int session_id = createApplicationSession(security_descriptor_id, addr_descriptor_id);
  if(NO_SESSION != session_id){
#if(UI_APPLICATION_COUNT>0)
    if(handleApplication(message, message_size, session_id, msg_application) > 0){
#else
    if(handleApplication(message, message_size, session_id) > 0){
#endif
      return session_id;
    }
  }
  return NO_SESSION;
}

unsigned int generateApplicationLayer(unsigned char * message, unsigned int message_size, unsigned char *buf, unsigned int max_buf_size){
  if(message!=NULL && buf!=NULL&& message_size < max_buf_size){
    memcpy(buf, (const void *)message, message_size);
    return message_size;   
  }
  return 0;
  
}
/*!
 * \}
 */
