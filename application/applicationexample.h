/*-
 * applicationexample.h - Example applications 
 *
 * Copyright 2015 Wenwen Chen
*/

/*!
 * \addtogroup    application
 * \{
 *
 * \file
 * \brief       Header definitions for the example applications
 *
 * \author      Wenwen Chen 
 */
#ifndef __APPLICATION_EXAMPLE_H__
#define __APPLICATION_EXAMPLE_H__

#include "applicationhandler.h"

#ifdef  __cplusplus
extern "C" {
#endif

  /*!
   * \brief Example application defination  
   */
  extern const application exampleapplication;
  /*!
   * \brief Ack application defination  
   */
  extern const application ackapplication;

  /*!
   * \brief Generate an example application
   *
   * \param security_descriptor_id descriptor id of the security issues
   * \param addr_descriptor_id     descriptor id of the address
   *
   * \return                       Response message size, which is 4
   */
  inline unsigned int __attribute__((always_inline))generateTest(unsigned int security_descriptor_id, unsigned int addr_descriptor_id){
    application_session *p_session = getApplicationSession(createApplicationSession(security_descriptor_id, addr_descriptor_id));
    if(p_session==NULL){
      return 0;
    }
    memcpy(p_session->message, "TEST", 4);
    p_session->message_size = 4;
    return 4;
  }

  /*!
   * \brief Generate an ack application
   *
   * \param security_descriptor_id descriptor id of the security issues
   * \param addr_descriptor_id     descriptor id of the address
   *
   * \return                       Response message size, which is 3
   */
  inline unsigned int __attribute__((always_inline))generateAckReq(unsigned int security_descriptor_id, unsigned int addr_descriptor_id){
    application_session *p_session = getApplicationSession(createApplicationSession(security_descriptor_id, addr_descriptor_id));
    if(p_session==NULL){
      return 0;
    }
    memcpy(p_session->message, "ACK", 3);
    p_session->message_size = 3;
    return 3;
  }

#ifdef  __cplusplus
}
#endif /* __cplusplus */

#endif /* __APPLICATION_EXAMPLE_H__ */
/*!
 * \}
 */
