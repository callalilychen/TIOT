/*-
 * applicationgreenled.c - Appliction for controlling green led on resource server 
 *
 * Copyright 2015 Wenwen Chen
 */

/*!
 * \addtogroup    constrained_rs_application 
 * \{
 *
 * \file
 * \brief       Implementation of the constrained resource server green led applications 
 *
 * \author      Wenwen Chen 
 */
#include "applicationgreenled.h"
#include "applicationhandler.h"


/*!
 * \brief Handle set green led on message
 *
 * \param req      Rest of request message after application name
 * \param req_size Size of this message
 * \param p_session pointer to the corresponding application session
 *
 * \return Response message size
 */
unsigned int handleGreenLEDon(unsigned char* req, unsigned int req_size, application_session * p_session);

const application greenledonapplication = {
  .name = "glon",
  .name_size = 4,
#if(MAX_APPLICATION_USAGE_SIZE>0)
  .usage = "\t\tTurn on green LED",
#endif
  .required_right = 0,
  .func = handleGreenLEDon
};
unsigned int handleGreenLEDon(unsigned char* req, unsigned int req_size, application_session * p_session){
  if(req_size < 0 || p_session == NULL){
    return 0;
  }
  GREEN_LED_ON;
  return generateApplicationStatusResponse(&greenledonapplication, p_session, DONE, NULL, 0);
  
}

/*!
 * \brief Handle set green led off message
 *
 * \param req      Rest of request message after application name
 * \param req_size Size of this message
 * \param p_session pointer to the corresponding application session
 *
 * \return Response message size
 */
unsigned int handleGreenLEDoff(unsigned char* req, unsigned int req_size, application_session * p_session);

const application greenledoffapplication = {
  .name = "gloff",
  .name_size = 5,
#if(MAX_APPLICATION_USAGE_SIZE>0)
  .usage = "\t\tTurn off green LED",
#endif
  .required_right = 0,
  .func = handleGreenLEDoff
};
unsigned int handleGreenLEDoff(unsigned char* req, unsigned int req_size, application_session * p_session){
  if(req_size < 0 || p_session == NULL){
    return 0;
  }
  GREEN_LED_OFF;
  return generateApplicationStatusResponse(&greenledoffapplication, p_session, DONE, NULL, 0);
}

/*!
 * \brief Handle getting led status message
 *
 * \param req      Rest of request message after application name
 * \param req_size Size of this message
 * \param p_session pointer to the corresponding application session
 *
 * \return Response message size
 */
unsigned int handleGreenLED(unsigned char* req, unsigned int req_size, application_session * p_session);
const application greenledapplication = {
  .name = "gls",
  .name_size = 3,
#if(MAX_APPLICATION_USAGE_SIZE>0)
  .usage = "\t\tGreen LED status",
#endif
  .required_right = 0,
  .func = handleGreenLED
};

unsigned int handleGreenLED(unsigned char* req, unsigned int req_size, application_session * p_session){
  if(req_size < 0 || p_session == NULL){
    return 0;
  }
  char * status_string;
  if(LED_IS_OFF == GREEN_LED_STATUS){
    status_string = "OFF";
  }else{
    status_string = "ON";
  }
  return generateApplicationStatusResponse(&greenledapplication, p_session, INFO, (unsigned char *)status_string, strlen(status_string));
}


/*!
 * \}
 */
