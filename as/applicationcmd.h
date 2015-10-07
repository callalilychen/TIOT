/*-
 * applicationcmd.h - Command appliction for authorization server 
 *
 * Copyright 2015 Wenwen Chen
*/

/*!
 * \addtogroup    as_application 
 * \{
 *
 * \file
 * \brief       Header definitions for the authorization server command applications 
 *
 * \author      Wenwen Chen 
 */
#ifndef __APPLICATION_CMD_H__
#define __APPLICATION_CMD_H__

#include "applicationhandler.h"

#ifdef  __cplusplus
extern "C" {
#endif

  extern const application keyapplication;
  extern const application lsaddrapplication;
  extern const application addaddrapplication;
  extern const application updateaddrapplication;
  
  unsigned int handleKey(unsigned char* , unsigned int, application_session *);
  unsigned int handleLsAddr(unsigned char* , unsigned int, application_session *);
  unsigned int handleAddAddr(unsigned char* , unsigned int, application_session *);
  unsigned int handleUpdateAddr(unsigned char* , unsigned int, application_session *);

#ifdef  __cplusplus
}
#endif /* __cplusplus */

#endif /* __APPLICATION_CMD_H__ */
/*!
 * \}
 */
