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
        memcpy(buf, SUCC_MESSAGE, DONE_MESSAGE_SIZE);
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
/*!
 * \}
 */
