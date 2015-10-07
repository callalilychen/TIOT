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
   * \brief Application defination of revocation
   */
  extern const application revapplication;
  /*!
   * \brief Application defination to set led on 
   */
  extern const application ledonapplication;
  /*!
   * \brief Application defination to set led off 
   */
  extern const application ledoffapplication;
  /*!
   * \brief Application defination to get led status 
   */
  extern const application ledapplication;
  
#ifdef  __cplusplus
}
#endif /* __cplusplus */

#endif /* __APPLICATION_MSG_H__ */
/*!
 * \}
 */
