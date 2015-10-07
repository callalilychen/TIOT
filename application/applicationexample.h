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

  extern const application exampleapplication;
  extern const application ackapplication;
  extern const application repapplication;
  
  unsigned int handleTest(unsigned char* , unsigned int, application_session *);
  unsigned int handleAckReq(unsigned char* , unsigned int, application_session *);
  unsigned int handleRep(unsigned char* , unsigned int, application_session *);

  inline unsigned int (__attribute__((always_inline))generateTest)(unsigned int next_layer_descriptor, unsigned int addr_descriptor){
    //TODO default session?
    unsigned char* res = addApplicationSession(4, next_layer_descriptor, addr_descriptor);
    if(res==NULL){
      return 0;
    }
    res[0] = 'T';
    res[1] = 'E';
    res[2] = 'S';
    res[3] = 'T';
    return 4;
  }

  inline unsigned int (__attribute__((always_inline))generateAckReq)(unsigned int next_layer_descriptor, unsigned int addr_descriptor){
    unsigned char* res = addApplicationSession(3, next_layer_descriptor, addr_descriptor);
    if(res==NULL){
      return 0;
    }
    res[0] = 'A';
    res[1] = 'C';
    res[2] = 'K';
    return 3;
  }

#ifdef  __cplusplus
}
#endif /* __cplusplus */

#endif /* __APPLICATION_EXAMPLE_H__ */
/*!
 * \}
 */
