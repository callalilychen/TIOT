/*-
 * applicationredled.c - Appliction for controlling red led on resource server 
 *
 * Copyright 2015 Wenwen Chen
 */

/*!
 * \addtogroup    constrained_rs_application 
 * \{
 *
 * \file
 * \brief       Implementation of the constrained resource server red led applications 
 *
 * \author      Wenwen Chen 
 */
#include "applicationredled.h"
#include "applicationhandler.h"


/*!
 * \brief Handle set red led on message
 *
 * \param req      Rest of request message after application name
 * \param req_size Size of this message
 * \param p_session pointer to the corresponding application session
 *
 * \return Response message size
 */
unsigned int handleRedLEDon(unsigned char* req, unsigned int req_size, application_session * p_session);

const application redledonapplication = {
  .name = "rlon",
  .name_size = 4,
#if(MAX_APPLICATION_USAGE_SIZE>0)
  .usage = "\t\tTurn on red led, required right 7",
#endif
  .required_right = 7,
  .func = handleRedLEDon
};
unsigned int handleRedLEDon(unsigned char* req, unsigned int req_size, application_session * p_session){
  if(req_size < 0 || p_session == NULL){
    return 0;
  }
  RED_LED_ON;
  return generateApplicationStatusResponse(&redledonapplication, p_session, SUCC, NULL, 0);
  
}

/*!
 * \brief Handle set red led off message
 *
 * \param req      Rest of request message after application name
 * \param req_size Size of this message
 * \param p_session pointer to the corresponding application session
 *
 * \return Response message size
 */
unsigned int handleRedLEDoff(unsigned char* req, unsigned int req_size, application_session * p_session);

const application redledoffapplication = {
  .name = "rloff",
  .name_size = 5,
#if(MAX_APPLICATION_USAGE_SIZE>0)
  .usage = "\t\tTurn off red led, required right 3",
#endif
  .required_right = 3,
  .func = handleRedLEDoff
};
unsigned int handleRedLEDoff(unsigned char* req, unsigned int req_size, application_session * p_session){
  if(req_size < 0 || p_session == NULL){
    return 0;
  }
  RED_LED_OFF;
  return generateApplicationStatusResponse(&redledoffapplication, p_session, SUCC, NULL, 0);
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
unsigned int handleRedLED(unsigned char* req, unsigned int req_size, application_session * p_session);
const application redledapplication = {
  .name = "rls",
  .name_size = 3,
#if(MAX_APPLICATION_USAGE_SIZE>0)
  .usage = "\t\tRed led status, required right 1",
#endif
  .required_right = 1,
  .func = handleRedLED
};

unsigned int handleRedLED(unsigned char* req, unsigned int req_size, application_session * p_session){
  if(req_size < 0 || p_session == NULL){
    return 0;
  }
  char * status_string;
  if(LED_IS_OFF == RED_LED_STATUS){
    status_string = " is off";
  }else{
    status_string = " is on";
  }
  return generateApplicationStatusResponse(&redledapplication, p_session, SUCC, status_string, strlen(status_string));
}


/*!
 * \}
 */
