/*-
 * applicationaddr.h - Address command appliction for authorization server 
 *
 * Copyright 2015 Wenwen Chen
*/

/*!
 * \addtogroup    as_application 
 * \{
 *
 * \file
 * \brief       Header definitions for the authorization server address command applications 
 *
 * \author      Wenwen Chen 
 */
#ifndef __APPLICATION_ADDR_H__
#define __APPLICATION_ADDR_H__

#include "applicationhandler.h"

#ifdef  __cplusplus
extern "C" {
#endif

  /*!
   * \brief Application defination to list all address descriptors 
   */
  extern const application lsaddrapplication;
  /*!
   * \brief Application defination to add an address
   */
  extern const application addaddrapplication;
  /*!
   * \brief Application defination to update an address descriptor
   */
  extern const application updateaddrapplication;
#ifdef  __cplusplus
}
#endif /* __cplusplus */

#endif /* __APPLICATION_ADDR_H__ */
/*!
 * \}
 */
