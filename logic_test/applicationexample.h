#ifndef __APPLICATION_EXAMPLE_H__
#define __APPLICATION_EXAMPLE_H__

#include "application_interface.h"

#ifdef  __cplusplus
extern "C" {
#endif

  extern const application exampleapplication;
  extern const application ackapplication;
  
  unsigned int handleTest(unsigned char* , unsigned int);
  unsigned int handleAckReq(unsigned char* , unsigned int);

  inline unsigned int (__attribute__((always_inline))generateTest)(){
    unsigned char* res = addApplicationLayerSession(4, NO_SESSION);
    if(res==NULL){
      return 0;
    }
    res[0] = 'T';
    res[1] = 'E';
    res[2] = 'S';
    res[3] = 'T';
    return 4;
  }

  inline unsigned int (__attribute__((always_inline))generateAckReq)(){
    unsigned char* res = addApplicationLayerSession(3, NO_SESSION);
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
