#include "applicationcmd.h"
#include "securitydescriptor.h"
#include "securitylayer.h"
#include "tree.h"



const application revapplication = {
  .name = "rev:",
  .name_size = 4,
  .required_right = NO_RIGHT,
  .func = handleRevocation
};

  unsigned int handleRevocation(unsigned char* req, unsigned int req_size, application_session * p_session){
    if(req_size < 0 || p_session == NULL){
      return 0;
    }
    unsigned int secret_index;
    if(SSCAN((const char*)req, "%u", &secret_index)==1){
      // add security layer descriptor
      p_session->next_layer_descriptor = PREDEF_NO_SECURITY_DESCRIPTOR;
      // TODO rev for rs
      // if(SUCC == setBit(secret_index)){
      //}else{
        SPRINT((char *)(p_session->message), "Error to Rev %d. S!", secret_index);
      //}
      p_session->message_size = strlen((const char *)(p_session->message));
      return p_session->message_size;
    }
    return 0;
  }


