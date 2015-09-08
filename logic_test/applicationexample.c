#include "applicationexample.h"

const application exampleapplication = {
  .name = "test",
  .func = handleTest
};

  unsigned int handleTest(unsigned char* res, unsigned int max_res_size){
    if(max_res_size < 2){
      return 0;
    }
    res[0] = 'O';
    res[1] = 'K';
    return 2;
  }
  
const application ackapplication = {
  .name = "ack",
  .func = handleAckReq
};
  unsigned int handleAckReq(unsigned char* res, unsigned int max_res_size){
    if(max_res_size < 3){
      return 0;
    }
    memcpy(res, &"ACK", 3);
    return 3;
  }
