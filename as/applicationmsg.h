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

  /*!
   * \brief Application defination for messages, which begin with '['
   */
  extern const application msgapplication;

  /*!
   * \brief Application defination of revocation
   */
  extern const application revapplication;

  /*!
   * \brief Application defination of permission request
   */
  extern const application permreqapplication;
 

#ifdef  __cplusplus
}
#endif /* __cplusplus */

#endif /* __APPLICATION_MSG_H__ */
/*!
 * \}
 */
