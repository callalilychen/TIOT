#include "applicationlayer.h"

applicationlayer_session applicationlayer_sessions[MAX_APPLICATION_LAYER_SESSION] = {0};

int handleApplicationLayer(unsigned char * payload, size_t payload_size, unsigned int header_session){
  if(payload_size < 1){
    return FAIL;
  }

  for(int i=0; i<MAX_APPLICATION_LAYER_SESSION; i++){
    if(applicationlayer_sessions[i].message_size==0){
      applicationlayer_sessions[i].next_header_session = header_session;
      applicationlayer_sessions[i].message_size = handleApplication(payload, payload_size, applicationlayer_sessions[i].message ,MAX_APPLICATION_MESSAGE_SIZE);
      return SUCC;   
    }
  }

  return FAIL;
}

unsigned char * generateApplicationLayer(unsigned int *p_size, unsigned int *session, unsigned int *p_next_layer_session){
  for(int i=0; i<MAX_APPLICATION_LAYER_SESSION; i++){
    if((*p_size = applicationlayer_sessions[i].message_size)>0){
      *session = i;
      *p_next_layer_session = applicationlayer_sessions[i].next_header_session;
      return applicationlayer_sessions[i].message;   
    }
  }
  return NULL;
  
}

void clearApplicationLayerSession(unsigned int session_index){
  if(session_index < MAX_APPLICATION_LAYER_SESSION){
    applicationlayer_sessions[session_index].message_size = 0;
    applicationlayer_sessions[session_index].next_header_session = NO_SESSION;
  }
}
