/*-
 * applicationmsg.h - Message appliction for authorization server 
 *
 * Copyright 2015 Wenwen Chen
*/

/*!
 * \addtogroup    as_application 
 * \{
 *
 * \file
 * \brief       Header definitions for the authorization server message applications 
 *
 * \author      Wenwen Chen 
 */
#ifndef __APPLICATION_MSG_H__
#define __APPLICATION_MSG_H__

#include "applicationhandler.h"

#ifdef  __cplusplus
extern "C" {
#endif

  extern const application revapplication;
  extern const application permreqapplication;
  
  unsigned int handleRevocation(unsigned char* , unsigned int, application_session *);
  unsigned int handlePermReq(unsigned char* , unsigned int, application_session *);

#ifdef  __cplusplus
}
#endif /* __cplusplus */

#endif /* __APPLICATION_MSG_H__ */
/*!
 * \}
 */
