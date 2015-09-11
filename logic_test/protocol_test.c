#include "testcase.h"

static void printBlock(char* name, unsigned char* block, size_t block_len){
  PRINT("%s:\n", name);
  for (int i=0; i< block_len; i++){
    PRINT("%x|", block[i]);
  }
  PRINT("\n");
}

static inline void receivedUdpPackage(unsigned char* udp_payload, unsigned int udp_payload_size){
  unsigned int security_descriptor = 0;
  unsigned int header_size = 0;
  if(NO_DESCRIPTOR != (security_descriptor = handleSecurityLayer(udp_payload, &udp_payload_size, &header_size))){
    handleApplicationLayer(udp_payload+header_size, udp_payload_size, security_descriptor);
  }
}

static inline unsigned int sendUdpPackage(unsigned char* send_buf, unsigned int max_send_buf){
  unsigned int  application_layer_msg_size = 0;
  unsigned char *  application_layer_msg = NULL;
  unsigned int buf_size = 0;
  unsigned char *buf = send_buf;
  unsigned int application_session = NO_SESSION;
  unsigned int security_descriptor = NO_SESSION;
  while(NULL!=(application_layer_msg = generateApplicationLayer(&application_session, & application_layer_msg_size, &security_descriptor))){
    buf_size = 0;
    buf = send_buf;
    buf_size = generateSecurityLayerHeader(security_descriptor, buf, max_send_buf);
    memcpy(buf+buf_size, application_layer_msg, application_layer_msg_size);
    buf_size +=application_layer_msg_size;
    buf_size +=generateSecurityLayerMAC(security_descriptor, application_layer_msg, application_layer_msg_size, buf, max_send_buf-buf_size);
    clearApplicationLayerSession(application_session); 
    removeSecurityLayerDescriptor(security_descriptor); 
  } 
  return buf_size;
}


int protocol_test(void){
  resetAllStates();
  unsigned char udp_payload[100] = "testrep";
  unsigned int udp_payload_size = (unsigned int)strlen((const char *)udp_payload);
  printf("len of %s is %d\n", udp_payload, udp_payload_size);

  udp_payload[0] = 0;

  const char root[5] = "test";
  unsigned char xoredRoot[4] = "eses";
  
  for(int i = 0; i < strlen(root); i++){
    xoredRoot[i] ^=  root[i];
  }
  unsigned char nodes[3][HASH_SIZE]={0};
  
  sha_construction.func(xoredRoot, strlen(root), nodes[0]);
  printBlock("Node 0", nodes[0], HASH_SIZE);

  for(int i = 0; i < HASH_SIZE; i+=2){
    nodes[0][i] ^=  udp_payload[3];
  }

  printBlock("xored Node 0", nodes[0], HASH_SIZE);
  HASH_FUNC(nodes[0], HASH_SIZE, nodes[1]);
  printBlock("Node 1", nodes[1], HASH_SIZE);

  unsigned int hmac_size =0;
  hmac(&sha_construction, nodes[1], HASH_SIZE,(const unsigned char*)udp_payload+4, udp_payload_size-4, nodes[2], &hmac_size);
  printBlock("Node 2", nodes[2], HASH_SIZE);
  
  memcpy(udp_payload+udp_payload_size, nodes[2], 20);
  printBlock("udp_payload", udp_payload, udp_payload_size+20);
  udp_payload_size += 20;
  
  setRoot((unsigned char *)root, 4);
  if(udp_payload_size > 0){
    receivedUdpPackage((unsigned char *)(udp_payload), udp_payload_size);
  }
  
  unsigned char send_buf[100] = {0};
  unsigned int send_buf_size = sendUdpPackage(send_buf, 100);

  if(send_buf_size == udp_payload_size && memcmp(send_buf, udp_payload, udp_payload_size)){
    PRINT("SEND AND RECEIVE TESTCASE: OK\n");
  }else{
    PRINT("SEND AND RECEIVE TESTCASE: FAIL\n");
  }d
}
