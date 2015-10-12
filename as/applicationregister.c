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
#include "applicationsecurity.h"
#include "applicationsend.h"
#include "applicationhelp.h"

const application *msg_applications[MSG_APPLICATION_COUNT] = {
  &exampleapplication, 
  &ackapplication, 
  &msgapplication,
  &revapplication, 
  &permreqapplication
};

#if(UI_APPLICATION_COUNT>0)
const application *ui_applications[UI_APPLICATION_COUNT] = {
  &exampleapplication, 
  &revapplication,
  &lsaddrapplication,
  &addaddrapplication,
  &editaddrapplication,
  &selectaddrapplication,
  &lssecapplication,
  &addsecapplication,
  &editprotocoltypeapplication,
  &editsecretindexapplication,
  &editpermapplication,
  &editkeyapplication,
  &selectsecapplication,
  &sendapplication,
  &securesendapplication,
  &testsendapplication,
  &testsecuresendapplication,
  &testconfigapplication,
  &testsettingapplication,
  &teststopapplication,
  &rshelpapplication,
  &msghelpapplication,
  &uihelpapplication
};
#endif

void initApplication(void){
  pthread_mutex_init(&(test.lock), NULL);
}
/*!
 * \}
 */
