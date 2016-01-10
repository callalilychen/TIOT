/*-
 * applicationregister - Register for applications for resource server  
 *
 * Copyright 2015 Wenwen Chen
*/

/*!
 * \defgroup    constrained_rs_application Applications for constrained resource server 
 * \{
 *
 * \file
 * \brief       Implementation of the security descriptor functions
 *
 * \author      Wenwen Chen 
 */

#include "applicationexample.h"
#include "applicationrsstate.h"
#include "applicationredled.h"
#include "applicationgreenled.h"

const application *msg_applications[MSG_APPLICATION_COUNT] = {
  &exampleapplication, 
  &rsrevapplication2,
  &redledonapplication,
  &redledoffapplication,
  &redledapplication,
  &greenledonapplication,
  &greenledoffapplication,
  &greenledapplication
};

void initApplication(void){
  // do nothing
}
/*!
 * \}
 */
