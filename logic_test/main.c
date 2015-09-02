#include <string.h>
#include <openssl/sha.h>
#include <openssl/hmac.h>
#include <openssl/evp.h>
#include <openssl/md5.h>
#include "interface.h"
#include "securitylayer.h" 
#include "applicationlayer.h" 


void printBlock(char* name, unsigned char* block, size_t block_len){
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
}
#define P_SEND &sendC
int main(int argc, char** argv)
{ 
  resetState();
  char udp_payload[100] = "testtesttesttestlalalalalalalalalalallalalalalalalalallalalalalalalalalalalalalalalal";
  udp_payload[0] = 0;
  udp_payload[1] = 0;

  char root[5] = "test";

  char xoredRoot[4];
  memcpy(xoredRoot, root, 4);
  xoredRoot[2] = 0;
  unsigned char nodes[3][HASH_BLOCK_LENGTH]={0};
  
  HASH_FUNC(&xoredRoot, 4, nodes[0]);
  printBlock("Node 0", nodes[0], HASH_BLOCK_LENGTH);

  for(int i = 0; i < HASH_BLOCK_LENGTH; i+=2){
    nodes[0][i] ^=  udp_payload[3];
  }

  printBlock("xored Node 0", nodes[0], HASH_BLOCK_LENGTH);
  HASH_FUNC(nodes[0], HASH_BLOCK_LENGTH, nodes[1]);
  printBlock("Node 1", nodes[1], HASH_BLOCK_LENGTH);

  MAC_FUNC(HASH_FUNC, HASH_BLOCK_LENGTH, nodes[1], HASH_BLOCK_LENGTH, udp_payload+4, 76, nodes[2]);
  printBlock("Node 2", nodes[2], HASH_BLOCK_LENGTH);
  
  memcpy(udp_payload+80, nodes[2], 20);
  
  setRoot(root, 4);
  size_t udp_payload_size = 100;
  printf("len of %s is %lu\n", udp_payload, udp_payload_size);
  unsigned char * payload;
  size_t payload_size = 0;
  uint8_t protocol_version = VERSION_BITS(udp_payload[0]);
  if(handleSecurityLayer(protocol_version, (unsigned char *)(udp_payload), udp_payload_size, &payload, &payload_size, sendBuf, &sendBufIdx) == SUCC){
    unsigned char * response_payload;
    size_t response_payload_size = 0;
    handleApplicationLayer(payload, payload_size, response_payload, &response_payload_size, sendBuf, &sendBufIdx);
    sendBufIdx = 0;
    handleSecurityLayerAfterPayload(protocol_version, response_payload, response_payload_size, sendBuf+sendBufIdx, &sendBufIdx);
  }
}
