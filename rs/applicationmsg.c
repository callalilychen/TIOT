/*-
 * applicationmsg.c - Message appliction for authorization server 
 *
 * Copyright 2015 Wenwen Chen
 */

/*!
 * \addtogroup    as_application 
 * \{
 *
 * \file
 * \brief       Implementation of the authorization server message applications 
 *
 * \author      Wenwen Chen 
 */
#include "applicationmsg.h"
#include "applicationhandler.h"
#include "treestate.h"

unsigned int handleRevocation(unsigned char* req, unsigned int req_size, application_session * p_session);
const application revapplication = {
  .name = "rev:",
  .name_size = 4,
  .required_right = ADMIN_RIGHT,
  .func = handleRevocation
};
/*!
 * \brief Handle revocation message
 *
 * \param req      Rest of request message after application name
 * \param req_size Size of this message
 * \param p_session pointer to the corresponding application session
 *
 * \return Response message size
 */
unsigned int handleRevocation(unsigned char* req, unsigned int req_size, application_session * p_session){
  if(req_size < 0 || p_session == NULL){
    return 0;
  }
  unsigned int secret_index;
  if(SSCAN((const char*)req, "%u", &secret_index)==1){
    p_session->message_size = generateApplicationHeader(p_session->message, MAX_APPLICATION_MESSAGE_SIZE-req_size - revapplication.name_size,invalidStateVector(secret_index));
    if(p_session->message_size > 0){
      memcpy(p_session->message + p_session->message_size, req-revapplication.name_size, req_size+revapplication.name_size);
      p_session->message_size +=  req_size + revapplication.name_size;
    }
  }
  return p_session->message_size;
}


unsigned int handleLEDon(unsigned char* req, unsigned int req_size, application_session * p_session);
const application ledonapplication = {
  .name = "lon:",
  .name_size = 4,
  .usage = "%u\tled nummer",
  .required_right = 7,
  .func = handleLEDon
};


/*!
 * \brief Handle set led on message
 *
 * \param req      Rest of request message after application name
 * \param req_size Size of this message
 * \param p_session pointer to the corresponding application session
 *
 * \return Response message size
 */
unsigned int handleLEDon(unsigned char* req, unsigned int req_size, application_session * p_session){
  if(req_size < 0 || p_session == NULL){
    return 0;
  }
  unsigned int led_index;
  if(SSCAN((const char*)req, "%u", &led_index)==1){
    p_session->message_size = generateApplicationHeader(p_session->message, MAX_APPLICATION_MESSAGE_SIZE-req_size - ledonapplication.name_size,  LED_ON(led_index));
    if(p_session->message_size > 0){
      memcpy(p_session->message + p_session->message_size, req-ledonapplication.name_size, req_size+ledonapplication.name_size);
      p_session->message_size +=  req_size + ledonapplication.name_size;
    }
  }
  return p_session->message_size;
}

unsigned int handleLEDoff(unsigned char* req, unsigned int req_size, application_session * p_session);
const application ledoffapplication = {
  .name = "loff:",
  .name_size = 5,
  .usage = "%u\tled nummer",
  .required_right = 3,
  .func = handleLEDoff
};

/*!
 * \brief Handle set led off message
 *
 * \param req      Rest of request message after application name
 * \param req_size Size of this message
 * \param p_session pointer to the corresponding application session
 *
 * \return Response message size
 */
unsigned int handleLEDoff(unsigned char* req, unsigned int req_size, application_session * p_session){
  if(req_size < 0 || p_session == NULL){
    return 0;
  }
  unsigned int led_index;
  if(SSCAN((const char*)req, "%u", &led_index)==1){
    p_session->message_size = generateApplicationHeader(p_session->message, MAX_APPLICATION_MESSAGE_SIZE-req_size - ledoffapplication.name_size,  LED_OFF(led_index));
    if(p_session->message_size > 0){
      memcpy(p_session->message + p_session->message_size, req-ledoffapplication.name_size, req_size+ledoffapplication.name_size);
      p_session->message_size +=  req_size + ledoffapplication.name_size;
    }
  }
  return p_session->message_size;

}

unsigned int handleLED(unsigned char* req, unsigned int req_size, application_session * p_session);
const application ledapplication = {
  .name = "led:",
  .name_size = 4,
  .usage = "%u\tled nummer",
  .required_right = 1,
  .func = handleLED
};

/*!
 * \brief Handle getting led status message
 *
 * \param req      Rest of request message after application name
 * \param req_size Size of this message
 * \param p_session pointer to the corresponding application session
 *
 * \return Response message size
 */
unsigned int handleLED(unsigned char* req, unsigned int req_size, application_session * p_session){
  if(req_size < 0 || p_session == NULL){
    return 0;
  }
  unsigned int led_index;
  if(SSCAN((const char*)req, "%u", &led_index)==1){
    char * status_string;
    int status = LED(led_index);
    if(NO_LED == status){
      p_session->message_size = generateApplicationHeader(p_session->message, MAX_APPLICATION_MESSAGE_SIZE-req_size - ledapplication.name_size,  FAIL);
      status_string = "";
    }else{
      if(LED_IS_ON == status){
        status_string = " is on";
      }else{
        status_string = " is off";
      }
      p_session->message_size = generateApplicationHeader(p_session->message, MAX_APPLICATION_MESSAGE_SIZE-req_size - ledapplication.name_size - strlen(status_string), INFO);
    }
    if(p_session->message_size > 0){
      memcpy(p_session->message + p_session->message_size, req-ledapplication.name_size, req_size+ledapplication.name_size);
      p_session->message_size +=  req_size + ledapplication.name_size;
      memcpy(p_session->message + p_session->message_size, status_string, strlen(status_string));
      p_session->message_size += strlen(status_string);
    }
  }
  return p_session->message_size;
}


/*!
 * \}
 */
