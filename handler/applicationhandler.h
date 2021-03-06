/*-
 * applicationhandler.h - Application handler 
 *
 * Copyright 2015 Wenwen Chen
 */

/*!
 * \addtogroup  handler
 * \{
 *
 * \file
 * \brief       Header definitions for the application session functions
 *
 * \author      Wenwen Chen 
 */
#ifndef __APPLICATION_HANDLER_H__
#define __APPLICATION_HANDLER_H__
#include <string.h>
#include "applicationsession.h" 
#include "securitylayerhandler.h"
#include "interface.h"
#ifdef  __cplusplus
extern "C" {
#endif
  /*! 
   * \brief Structure for defination an application
   *
   * \note The name of each application should be unique
   */
  typedef struct application{
    const unsigned char name[MAX_APPLICATION_NAME_SIZE];
    const unsigned int name_size;
#if(MAX_APPLICATION_USAGE_SIZE>0)
    const unsigned char usage[MAX_APPLICATION_USAGE_SIZE];
#endif
    const RIGHT_TYPE required_right;
    unsigned int (*func)(unsigned char*, unsigned int, application_session *);
  }application;

  /*! 
   * \brief List of supported application, which can be required using a message
   *
   *        The id of applications in the list is the index of each application
   *
   * \note The name of each application should be unique
   */
  extern const application * msg_applications[MSG_APPLICATION_COUNT];
#if(UI_APPLICATION_COUNT>0)
  /*! 
   * \brief List of supported application, which can be required via user interface
   *
   *        The id of applications in the list is MSG_APPLICATION_COUNT + the index of each application
   *
   * \note  In order to make the application id unique, the type of applications is also used to construct a application id
   */
  extern const application * ui_applications[UI_APPLICATION_COUNT];

  enum{msg_application = 0, ui_application};
#else
#define applications msg_applications
#define applications_len MSG_APPLICATION_COUNT
#endif

  /*! 
   * \brief handle a application request 
   *
   * \param req       Request message after application name
   * \param req_size  Size of this message
   * \param session_id The corresponding application session
   * \param application_type The type of required application, which is needed, if UI_APPLICATION_COUNT is bigger than 0
   *
   * \note  In order to make the application id unique, the type of applications is also used to construct a application id
   *
   * \return Application response message size
   */
#if(UI_APPLICATION_COUNT>0)
   unsigned int handleApplication(unsigned char* req, unsigned int req_size, unsigned int session_id, int application_type);
#else
     unsigned int handleApplication(unsigned char* req, unsigned int req_size, unsigned int session_id);
#endif

    /*! 
     * \brief Write application header to the given buf, according to given type 
     *
     * \param buf       Buffer to be written
     * \param buf_size  Size of this buffer
     * \param type      Type of the header type
     *
     * \return Application header size
     */
    unsigned int generateApplicationHeader(unsigned char* buf, unsigned int max_buf_size,  int type);

    /*! 
     * \brief Write application status response to the given application_session 
     *
     * \param p_application Pointer to the application
     * \param p_session     Pointer to the application session
     * \param type          Type of the header type
     * \param status       Status information
     * \param status_size  Size of the information
     *
     * \return application response size
     */
    unsigned int generateApplicationStatusResponse(const application* p_application, application_session * p_session, int type, unsigned char * status, unsigned int status_size);

    /*! 
     * \brief Init function for applications 
     *
     * \return None
     */
    void initApplication(void);
#ifdef  __cplusplus
  }
#endif /* __cplusplus */

#endif /* __APPLICATION_HANDLER_H__ */
  /*!
   * \}
   */
