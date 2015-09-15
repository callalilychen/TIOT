#include <string.h>
#include <openssl/sha.h>
#include <openssl/hmac.h>
#include <openssl/evp.h>
#include <openssl/md5.h>
#include <sys/types.h>
#include <stdio.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <unistd.h>
#include <pthread.h>
#include <arpa/inet.h>
#include "treestate.h"
#include "tree.h" 
#include "hmac.h"
#include "applicationexample.h"
#include "securitylayer.h" 
#include "applicationlayer.h" 


#include "utils.h"

#define PORT 9999

#define BUFSIZE 100

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

int main(int argc, char** argv)
{ 
  resetAllExpectedStates();
  unsigned char udp_payload[BUFSIZE] = {0};
  unsigned int udp_payload_size = 0;
  unsigned char send_buf[BUFSIZE] = {0};
  unsigned int send_buf_size = 0;
  

  // Create a UDP socket and listen on a port
  struct sockaddr_in si_me;
  int fd;
  if((fd = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP))==-1){
    perror("Can not create socket\n");
    return 0;
  }
  
  const char root[5] = "test";
  
  setRoot((unsigned char *)root, 4);
  
  si_me.sin_family = AF_INET;
  si_me.sin_addr.s_addr = htonl(INADDR_ANY);
  si_me.sin_port = htons(PORT);
  
  if(bind(fd, (struct sockaddr*)&si_me, sizeof(si_me))<0){
    perror("bind failed");
    return 0;
  }
  struct sockaddr_in si_remote;
  socklen_t addrlen = sizeof(si_remote);
  while(1){
    udp_payload_size = recvfrom(fd, udp_payload, BUFSIZE, 0, (struct sockaddr *)&si_remote, &addrlen);
    if(udp_payload_size>0){
      printf("received message: %s from %s:%d\n",udp_payload, inet_ntoa(si_remote.sin_addr), si_remote.sin_port);

      receivedUdpPackage(udp_payload, udp_payload_size);
    }
    send_buf_size = sendUdpPackage(send_buf, BUFSIZE);
    if(send_buf_size>0){
     sendto(fd, send_buf, send_buf_size, 0, (struct sockaddr *)&si_remote, addrlen); 
    }

  
  }
}
