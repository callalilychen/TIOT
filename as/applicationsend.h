/*-
 * applicationsend.h - Send command appliction for authorization server 
 *
 * Copyright 2015 Wenwen Chen
 */

/*!
 * \addtogroup    as_application 
 * \{
 *
 * \file
 * \brief       Header definitions for the authorization server send command applications 
 *
 * \author      Wenwen Chen 
 */
#ifndef __APPLICATION_SEND_H__
#define __APPLICATION_SEND_H__

#include "applicationhandler.h"
#include <stdint.h>
#include <stdio.h>
#include <pthread.h>
#include <time.h>
#ifdef  __cplusplus
extern "C" {
#endif

  /*!
   * \brief Application defination for messages, which begin with '['
   */
  extern const application msgapplication;
  /*!
   * \brief Currently selected address descriptor id
   */
  extern unsigned int selected_addr_id;
  /*!
   * \brief Currently selected security descriptor id 
   */
  extern unsigned int selected_security_id;

  /*!
   * \brief Application defination for send command without security issues 
   */
  extern const application sendapplication;
  /*!
   * \brief Application defination for send command with security issues 
   */
  extern const application securesendapplication;
  /*!
   * \brief Application defination for test send command without security issues 
   */
  extern const application testsendapplication;
  /*!
   * \brief Application defination for test send command with security issues 
   */
  extern const application testsecuresendapplication;
  /*!
   * \brief Application defination for configure test 
   */
  extern const application testconfigapplication;
  /*!
   * \brief Application defination for request of test setting
   */
  extern const application testsettingapplication;
  /*!
   * \brief Application defination to stop test 
   */
  extern const application teststopapplication;

/*!
 * \brief Macro defined maximal number of test send message 
 */
#define MAX_TEST_MESSAGE 4
/*!
 * \brief Macro defined status for test, when it is running  
 */
#define TEST_RUNNING 1
#define TEST_IDLE 0
  /*!
   * \brief Structure of a test setting 
   */
  typedef struct test_setting{
    unsigned int status;
    pthread_t thread;
    pthread_mutex_t lock;
    int send_counter;
    int recv_counter;
    struct timespec interval;
    unsigned int times;
    unsigned char buf[MAX_TEST_MESSAGE][BUFSIZE+1];
    unsigned int buf_size[MAX_TEST_MESSAGE];
    unsigned int addr_descriptor_id;
    unsigned int security_descriptor_id;
    //void (*sleep_function)(unsigned int);
  }test_setting;

  /*!
   * \brief Test setting instance
   */
  extern test_setting test;
#ifdef  __cplusplus
}
#endif /* __cplusplus */

#endif /* __APPLICATION_SEND_H__ */
/*!
 * \}
 */
