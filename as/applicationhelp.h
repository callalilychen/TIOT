/*-
 * applicationhelp.h - Helper appliction 
 *
 * Copyright 2015 Wenwen Chen
*/

/*!
 * \addtogroup    as_application 
 * \{
 *
 * \file
 * \brief       Header definitions for the helper applications 
 *
 * \author      Wenwen Chen 
 */
#ifndef __APPLICATION_HELPER_H__
#define __APPLICATION_HELPER_H__

#include "applicationhandler.h"

#ifdef  __cplusplus
extern "C" {
#endif

  /*!
   * \brief Application defination of user interface application helper
   */
  extern const application uihelpapplication;

  /*!
   * \brief Application defination of message application helper
   */
  extern const application msghelpapplication;

  /*!
   * \brief Application defination of resource server application helper
   */
  extern const application rshelpapplication;
 

#ifdef  __cplusplus
}
#endif /* __cplusplus */

#endif /* __APPLICATION_HELPER_H__ */
/*!
 * \}
 */
