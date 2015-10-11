/*-
 * applicationsecurity.h - Security appliction for authorization server 
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
#ifndef __APPLICATION_SECURITY_H__
#define __APPLICATION_SECURITY_H__

#include "applicationhandler.h"

#ifdef  __cplusplus
extern "C" {
#endif

  /*!
   * \brief Application defination for 
   */
extern const application lssecapplication;
extern const application addsecapplication;
extern const application editprotocoltypeapplication;
extern const application editsecretindexapplication;
extern const application editpermapplication;
extern const application editkeyapplication;
extern const application selectsecapplication;

#ifdef  __cplusplus
}
#endif /* __cplusplus */

#endif /* __APPLICATION_SECURITY_H__ */
/*!
 * \}
 */
