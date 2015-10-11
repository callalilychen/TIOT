/*-
 * applicationgreenled.h - Appliction for controlling green led on resource server 
 *
 * Copyright 2015 Wenwen Chen
*/

/*!
 * \addtogroup    constrained_rs_application 
 * \{
 *
 * \file
 * \brief       Header definitions for the constrained resource server green led applications 
 *
 * \author      Wenwen Chen 
 */
#ifndef __APPLICATION_GREEN_LED_H__
#define __APPLICATION_GREEN_LED_H__

#include "applicationhandler.h"

#ifdef  __cplusplus
extern "C" {
#endif

  /*!
   * \brief Application defination to set led on 
   */
  extern const application greenledonapplication;
  /*!
   * \brief Application defination to set led off 
   */
  extern const application greenledoffapplication;
  /*!
   * \brief Application defination to get led status 
   */
  extern const application greenledapplication;
  
#ifdef  __cplusplus
}
#endif /* __cplusplus */

#endif /* __APPLICATION_GREEN_LED_H__ */
/*!
 * \}
 */
