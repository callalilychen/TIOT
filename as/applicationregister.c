/*-
 * applicationregister - Register for applications for authorization server  
 *
 * Copyright 2015 Wenwen Chen
 */

/*!
 * \defgroup    as_application Applications for authorization server 
 * \{
 *
 * \file
 * \brief       Implementation of the security descriptor functions
 *
 * \author      Wenwen Chen 
 */

#include "applicationexample.h"
#include "applicationcmd.h"
#include "applicationaddr.h"
#include "applicationmsg.h"
#include "applicationhelp.h"

const application *msg_applications[MSG_APPLICATION_COUNT] = {
  &exampleapplication, 
  &ackapplication, 
  &msgapplication,
  &keyapplication, 
  &revapplication, 
  &permreqapplication
};

#if(UI_APPLICATION_COUNT>0)
const application *ui_applications[UI_APPLICATION_COUNT] = {
  &exampleapplication, 
  &lsaddrapplication,
  &addaddrapplication,
  &updateaddrapplication,
  &keyapplication, 
  &revapplication,
  &rshelpapplication,
  &msghelpapplication,
  &uihelpapplication
};
#endif
/*!
 * \}
 */
