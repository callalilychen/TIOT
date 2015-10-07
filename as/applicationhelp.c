/*-
 * applicationhelp.c - Helper appliction 
 *
 * Copyright 2015 Wenwen Chen
 */

/*!
 * \addtogroup    as_application 
 * \{
 *
 * \file
 * \brief       Implementation of the helper applications 
 *
 * \author      Wenwen Chen 
 */
#include "applicationhelp.h"
#include "applicationexample.h"
/*!
 * \brief Handle help request for user interface
 *
 * \note This application is only for ui, therefore, output is directly printed on the standard output via PRINT.
 *
 * \param req       Rest of request message after application name
 * \param req_size  Size of this message
 * \param p_session pointer to the corresponding application session
 *
 * \return         0 
 */
unsigned int handleUIHelp(unsigned char* req, unsigned int req_size, application_session * p_session){
  PRINT("%s %u user interface applications:\n", USAGE_MESSAGE, UI_APPLICATION_COUNT);
  for(int i = 0; i < UI_APPLICATION_COUNT; i++){
    PRINT("\t%s%s\n", ui_applications[i]->name, ui_applications[i]->usage);
  } 
  return 0;
}

const application uihelpapplication = {
  .name = "help",
  .name_size = 4,
  .usage = "\t\tUsage list of ui commands",
  .required_right = NO_RIGHT,
  .func = handleUIHelp
};

/*!
 * \brief Handle help request for message
 *
 * \note This application is only for ui, therefore, output is directly printed on the standard output via PRINT.
 *
 * \param req       Rest of request message after application name
 * \param req_size  Size of this message
 * \param p_session pointer to the corresponding application session
 *
 * \return         0 
 */
unsigned int handleMsgHelp(unsigned char* req, unsigned int req_size, application_session * p_session){
  PRINT("%s %u message applications:\n", USAGE_MESSAGE, MSG_APPLICATION_COUNT);
  for(int i = 0; i < MSG_APPLICATION_COUNT; i++){
    PRINT("\t%s%s\n", msg_applications[i]->name, msg_applications[i]->usage);
  } 
  return 0;
}

const application msghelpapplication = {
  .name = "helpmsg",
  .name_size = 7,
  .usage = "\t\tUsage list of messages",
  .required_right = NO_RIGHT,
  .func = &handleMsgHelp
};

/*!
 * \todo
 * TODO
 */
const static application rs_revapplication = {
  .name = "rev:",
  .name_size = 4,
  .usage = "%u\tsecret id",
  .required_right = ADMIN_RIGHT,
  .func = NULL
};
const static application rs_ledonapplication = {
  .name = "lon:",
  .name_size = 4,
  .usage = "%u\tled nummer",
  .required_right = 7,
  .func = NULL
};
const static application rs_ledoffapplication = {
  .name = "loff:",
  .name_size = 5,
  .usage = "%u\tled nummer",
  .required_right = 3,
  .func = NULL
};
const static application rs_ledapplication = {
  .name = "led:",
  .name_size = 4,
  .usage = "%u\tled nummer",
  .required_right = 1,
  .func = NULL
};

/*!
 * \brief Clone of RS message application register 
 */
const static application *rs_msg_applications[RS_MSG_APPLICATION_COUNT] = {
  &exampleapplication, 
  &ackapplication, 
  &rs_revapplication,
  &rs_ledonapplication,
  &rs_ledoffapplication,
  &rs_ledapplication
};

/*!
 * \brief Handle help request for message
 *
 * \note This application is only for ui, therefore, output is directly printed on the standard output via PRINT.
 *
 * \param req       Rest of request message after application name
 * \param req_size  Size of this message
 * \param p_session pointer to the corresponding application session
 *
 * \return         0 
 */
unsigned int handleRSHelp(unsigned char* req, unsigned int req_size, application_session * p_session){
  PRINT("%s %u resource server message applications:\n", USAGE_MESSAGE, RS_MSG_APPLICATION_COUNT);
  for(int i = 0; i < RS_MSG_APPLICATION_COUNT; i++){
    PRINT("\t%s%s\n", rs_msg_applications[i]->name, rs_msg_applications[i]->usage);
  } 
  return 0;
}

const application rshelpapplication = {
  .name = "helprs",
  .name_size = 6,
  .usage = "\t\tUsage list of resource server messages",
  .required_right = NO_RIGHT,
  .func = &handleRSHelp
};



/*!
 * \}
 */
