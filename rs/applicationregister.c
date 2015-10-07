/*-
 * applicationregister - Register for applications for resource server  
 *
 * Copyright 2015 Wenwen Chen
*/

/*!
 * \defgroup    rs_application Applications for resource server 
 * \{
 *
 * \file
 * \brief       Implementation of the security descriptor functions
 *
 * \author      Wenwen Chen 
 */

#include "applicationexample.h"
#include "applicationmsg.h"

const application *msg_applications[MSG_APPLICATION_COUNT] = {
  &exampleapplication, 
  &ackapplication, 
  &revapplication,
  &ledapplication,
  &ledapplication,
  &ledapplication
};

/*!
 * \}
 */
