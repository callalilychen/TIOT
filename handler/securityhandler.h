/*-
 * securityhandler.h - Security handler 
 *
 * Copyright 2005 Wenwen Chen
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
 * \brief Macro for when no right is required 
 */
#define NO_RIGHT 0x0

  /*!
   * \brief Check, whether hasRight is approved according to hasRight
   *
   * \parm hasRight      The right to be checked
   *       requiredRight  Required right
   *
   * \return              1 if approved, otherwise 0
   */
  inline unsigned int (__attribute__((always_inline))checkRight)(RIGHT_TYPE hasRight, RIGHT_TYPE requiredRight){
    return requiredRight == NO_RIGHT || hasRight ^ requiredRight;
  }
#ifdef  __cplusplus
}
#endif /* __cplusplus */

#endif /* __SECURITY_HANDLER_H__ */
/*!
 * \}
 */
