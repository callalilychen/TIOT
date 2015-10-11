/*-
 * securityhandler.h - Security handler 
 *
 * Copyright 2015 Wenwen Chen
*/

/*!
 * \addtogroup  handler
 * \{
 *
 * \file
 * \brief       Header definitions for the security functions
 *
 * \author      Wenwen Chen 
 */
#ifndef __SECURITY_HANDLER_H__
#define __SECURITY_HANDLER_H__
#include <stdint.h> 
#include "treeconfig.h"
#ifdef  __cplusplus
extern "C" {
#endif

  /*!
   * \brief Check, whether hasRight is approved according to hasRight
   *
   * \parm hasRight      The right to be checked
   *       requiredRight  Required right
   *
   * \return              1 if approved, otherwise 0
   */
  inline unsigned int (__attribute__((always_inline))checkRight)(RIGHT_TYPE hasRight, RIGHT_TYPE requiredRight){
    return requiredRight == NO_RIGHT || (requiredRight == (hasRight & requiredRight));
  }

#ifdef ADMIN_PASSWORD_HASH
  RIGHT_TYPE askForAdminRight(void);
#endif

#ifdef  __cplusplus
}
#endif /* __cplusplus */

#endif /* __SECURITY_HANDLER_H__ */
/*!
 * \}
 */
