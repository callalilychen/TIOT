#ifndef __APPLICATION_CMD_H__
#define __APPLICATION_CMD_H__

#include "application_interface.h"

#ifdef  __cplusplus
extern "C" {
#endif

  extern const application keyapplication;
  extern const application revapplication;
  extern const application permreqapplication;
  
  unsigned int handleKey(unsigned char* , unsigned int, application_session *);
  unsigned int handleRevocation(unsigned char* , unsigned int, application_session *);
  unsigned int handlePermReq(unsigned char* , unsigned int, application_session *);

//  inline unsigned int (__attribute__((always_inline))generateKey)(unsigned int next_layer_descriptor){
//    //TODO default session?
//    unsigned char* res = addApplicationSession(4, next_layer_descriptor);
//    if(res==NULL){
//      return 0;
//    }
//    res[0] = 'T';
//    res[1] = 'E';
//    res[2] = 'S';
//    res[3] = 'T';
//    return 4;
//  }
#ifdef  __cplusplus
}
#endif /* __cplusplus */

#endif /* __APPLICATION_CMD_H__ */
