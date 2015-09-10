#include "applicationlayer.h"
#include "application_interface.h"

application_session application_sessions[MAX_APPLICATION_SESSION] = {0};

unsigned int handleApplicationLayer(unsigned char * payload, unsigned int payload_size, unsigned int next_layer_descriptor){
  if(payload_size < 1){
    return NO_SESSION;
  }

  for(int i=0; i<MAX_APPLICATION_SESSION; i++){
    if(application_sessions[i].application_id==NO_APPLICATION){
      application_sessions[i].next_layer_descriptor = next_layer_descriptor;
      application_sessions[i].message_size = handleApplication(payload, payload_size, &application_sessions[i]);
      return i+1;   
    }
  }

  return NO_SESSION;
}

unsigned char * generateApplicationLayer(unsigned int *session, unsigned int *p_size, unsigned int *p_next_layer_descritpor){
  for(int i=0; i<MAX_APPLICATION_SESSION; i++){
    if((*p_size = application_sessions[i].message_size)>0){
      *session = i+1;
      *p_next_layer_descritpor = application_sessions[i].next_layer_descriptor;
      return application_sessions[i].message;   
    }
  }
  return NULL;
  
}
