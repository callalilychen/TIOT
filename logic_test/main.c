#include <string.h>
#include <openssl/sha.h>
#include <openssl/hmac.h>
#include <openssl/evp.h>
#include <openssl/md5.h>
#include "treestate.h"
#include "tree.h" 
#include "hmac.h"
#include "securitylayer.h" 
#include "applicationlayer.h" 
#include "testcase.h"


#include "utils.h"


static void printBlock(char* name, unsigned char* block, size_t block_len){
  PRINT("%s:\n", name);
  for (int i=0; i< block_len; i++){
    PRINT("%x|", block[i]);
  }
  PRINT("\n");
}
unsigned char sendBuf[100];
size_t sendBufIdx = 0;

int sendC(unsigned char c){
  if(sendBufIdx < 100){
  putchar(c);
  sendBuf[sendBufIdx++] = c;
  } else{
    printf("Too long!\n");
  }
  return 1;
}
#define P_SEND &sendC

static inline void receivedUdpPackage(unsigned char* udp_payload, unsigned int *p_udp_payload_size){
  uint8_t protocol_version = VERSION_BITS(udp_payload[0]);
  unsigned int header_size = 0;
  if(SUCC == handleSecurityLayer(protocol_version, udp_payload, p_udp_payload_size, &header_size)){
    handleApplicationLayer(udp_payload+header_size, *p_udp_payload_size,0);
  }
}

static inline void sendUdpPackage(void){
  unsigned int application_layer_msg_size = 0;
  unsigned char * application_layer_msg = NULL;
  unsigned int application_layer_session = NO_SESSION;
  unsigned int security_layer_session = NO_SESSION;
  while((application_layer_msg = generateApplicationLayer(&application_layer_msg_size, &application_layer_session, &security_layer_session)) != NULL){
    generateSecurityLayerHeader(security_layer_session);
    unsigned char tmp[100] = {0};
    memcpy(tmp, application_layer_msg, application_layer_msg_size);
    PRINT("get %s\n", tmp);
    generateSecurityLayerMAC(security_layer_session);
    clearApplicationLayerSession(application_layer_session); 
    clearSecurityLayerSession(security_layer_session); 
  } 
}

int main(int argc, char** argv)
{ 
  resetState();
  unsigned char udp_payload[100] = "testtest";
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
  
  memcpy(udp_payload+8, nodes[2], 20);
  udp_payload_size += 20;
  
  setRoot((unsigned char *)root, 4);
  if(udp_payload_size > 0){
    receivedUdpPackage((unsigned char *)(udp_payload), &udp_payload_size);
  }
  sendUdpPackage();
}
