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
#include "addr_descriptor.h"


#include "utils.h"

#define PORT 9999

#define BUFSIZE 100
socklen_t addrlen = sizeof(ADDR_TYPE);

static void printBlock(char* name, unsigned char* block, size_t block_len){
  PRINT("%s:\n", name);
  for (int i=0; i< block_len; i++){
    PRINT("%x|", block[i]);
  }
  PRINT("\n");
}


static inline void handleUdpPackage(unsigned char* udp_payload, unsigned int udp_payload_size, ADDR_TYPE *p_addr){
  unsigned int header_size = 0;
  unsigned int security_descriptor = handleSecurityLayer(udp_payload, &udp_payload_size, &header_size);
  if(udp_payload_size > 0){
    unsigned int addr_descriptor = addAddrDescriptors(p_addr, (size_t)addrlen);
    if(NO_DESCRIPTOR == addr_descriptor){
      PRINT("[ERROR] Failed to add the addr descritptor!\n");
    }else if(NO_SESSION == handleApplicationLayer(udp_payload+header_size, udp_payload_size, security_descriptor, addr_descriptor)){
      udp_payload[udp_payload_size+header_size] = 0;
      PRINT("[WARN] No application session for message \"%s\"!\n", (const char *)(udp_payload+header_size));
    }
  }
}

static inline unsigned int generateUdpPackage(unsigned char* send_buf, unsigned int max_send_buf){
  unsigned int  application_layer_msg_size = 0;
  unsigned char *  application_layer_msg = NULL;
  unsigned int send_buf_size = 0;
  unsigned int application_session = NO_SESSION;
  unsigned int security_descriptor = NO_SESSION;
  while(NULL!=(application_layer_msg = generateApplicationLayer(&application_session, & application_layer_msg_size, &security_descriptor))){
    send_buf_size = 0;
    send_buf_size = generateSecurityLayerHeader(security_descriptor, send_buf, max_send_buf);
    memcpy(send_buf+send_buf_size, application_layer_msg, application_layer_msg_size);
    send_buf_size += (application_layer_msg_size+
        generateSecurityLayerMAC(security_descriptor, send_buf+send_buf_size, application_layer_msg_size, max_send_buf-send_buf_size));
    clearApplicationSession(application_session); 
    removeSecurityLayerDescriptor(security_descriptor); 
  } 
  return send_buf_size;
}

int main(int argc, char** argv)
{ 
  resetAllExpectedStates();
  initApplicationSession();
  initSecurityDescriptor();

  unsigned char udp_payload[BUFSIZE] = {0};
  unsigned int udp_payload_size = 0;
  unsigned char send_buf[BUFSIZE] = {0};
  unsigned int send_buf_size = 0;
  
  const char root[5] = "test";
  setRoot((unsigned char *)root, 4);
  

  // Create a UDP socket and listen on a port
  struct sockaddr_in si_me;
  int fd;
  if((fd = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP))==-1){
    perror("Can not create socket\n");
    return 0;
  }
  
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

      handleUdpPackage(udp_payload, udp_payload_size, (ADDR_TYPE *)(&si_remote));
    }
    send_buf_size = generateUdpPackage(send_buf, BUFSIZE);
    if(send_buf_size>0){
      sendto(fd, send_buf, send_buf_size, 0, (struct sockaddr *)&si_remote, addrlen); 
      printBlock("send", send_buf, send_buf_size);

    }

  
  }
}
