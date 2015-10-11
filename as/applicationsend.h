/*-
 * applicationsend.h - Send command appliction for authorization server 
 *
 * Copyright 2015 Wenwen Chen
 */

/*!
 * \addtogroup    as_application 
 * \{
 *
 * \file
 * \brief       Header definitions for the authorization server send command applications 
 *
 * \author      Wenwen Chen 
 */
#ifndef __APPLICATION_SEND_H__
#define __APPLICATION_SEND_H__

#include "applicationhandler.h"

#ifdef  __cplusplus
extern "C" {
#endif

  /*!
   * \brief Currently selected address descriptor id
   */
  extern unsigned int selected_addr_id;
  /*!
   * \brief Currently selected security descriptor id 
   */
  extern unsigned int selected_security_id;

  /*!
   * \brief Application defination for send command without security issues 
   */
  extern const application sendtoapplication;
  /*!
   * \brief Application defination for send command with security issues 
   */
  extern const application securesendtoapplication;

#ifdef  __cplusplus
}
#endif /* __cplusplus */

#endif /* __APPLICATION_SEND_H__ */
/*!
 * \}
 */
