/*-
 * applicationredled.h - Appliction for controlling red led on resource server 
 *
 * Copyright 2015 Wenwen Chen
*/

/*!
 * \addtogroup    constrained_rs_application 
 * \{
 *
 * \file
 * \brief       Header definitions for the constrained resource server red led applications 
 *
 * \author      Wenwen Chen 
 */
#ifndef __APPLICATION_RED_LED_H__
#define __APPLICATION_RED_LED_H__

#include "applicationhandler.h"

#ifdef  __cplusplus
extern "C" {
#endif

  /*!
   * \brief Application defination to set led on 
   */
  extern const application redledonapplication;
  /*!
   * \brief Application defination to set led off 
   */
  extern const application redledoffapplication;
  /*!
   * \brief Application defination to get led status 
   */
  extern const application redledapplication;
  
#ifdef  __cplusplus
}
#endif /* __cplusplus */

#endif /* __APPLICATION_RED_LED_H__ */
/*!
 * \}
 */
