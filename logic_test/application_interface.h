#ifndef __APPLICATION_INTERFACE_H__
#define __APPLICATION_INTERFACE_H__

#include <string.h>

#ifdef  __cplusplus
extern "C" {
#endif

//TODO somewhere else
#define NO_SESSION 0xffffffff
#define MAX_APPLICATION_LAYER_SESSION 2
#define MAX_APPLICATION_MESSAGE_SIZE 10

  typedef struct applicationlayer_session{
    unsigned int next_header_session;
    // TODO header
    unsigned char message[MAX_APPLICATION_MESSAGE_SIZE];
    unsigned int message_size;
  }applicationlayer_session;

  extern applicationlayer_session applicationlayer_sessions[MAX_APPLICATION_LAYER_SESSION];

  inline unsigned char* (__attribute__((always_inline))addApplicationLayerSession)(unsigned int message_size, unsigned int next_header_session){
    if(message_size > MAX_APPLICATION_MESSAGE_SIZE){
      return NULL;
    }
    for(int i=0; i<MAX_APPLICATION_LAYER_SESSION; i++){
      if(applicationlayer_sessions[i].message_size==0){
        applicationlayer_sessions[i].message_size = message_size;
        applicationlayer_sessions[i].next_header_session = next_header_session;
        return applicationlayer_sessions[i].message;
      }
    }

    return NULL;
  }

#define MAX_APPLICATION_COUNTER 2
#define MAX_APPLICATION_NAME 4

  typedef struct application{
    unsigned char name[MAX_APPLICATION_NAME];
    unsigned int (*func)(unsigned char*, unsigned int);
  }application;

  extern const application * applications[MAX_APPLICATION_COUNTER];

  inline unsigned int (__attribute__((always_inline))handleApplication)(unsigned char* req, unsigned int req_size, unsigned char* res, unsigned int max_res_size){
    if(req_size >=3  && memcmp(&"req", req, 3)==0){
      memcpy(res, req, (size_t)req_size);
      return req_size;
    }
    
    for(int i = 0; i < MAX_APPLICATION_COUNTER; i++){
      if(req_size <= MAX_APPLICATION_NAME && memcmp(applications[i]->name, req, req_size)==0){
        return applications[i]->func(res, max_res_size); 
      } 
    }
    return 0;
  }

#ifdef  __cplusplus
}
#endif /* __cplusplus */

#endif /* __APPLICATION_INTERFACE_H__ */

