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

#include "packagehandler.h"

#define PORT 5001 
#define AS_PORT 9002
#define AS_IP "127.0.0.1"
#define BUFSIZE 100


int main(int argc, char** argv)
{ 
  resetAllExpectedStates();
  initApplicationSession();
  initSecurityDescriptors();

  unsigned char udp_payload[BUFSIZE+1] = {0};
  unsigned int udp_payload_size = 0;
  unsigned char send_buf[BUFSIZE+1] = {0};
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
  
  si_me.sin_family = ADDR_FAMILY;
  si_me.sin_addr.s_addr = htonl(INADDR_ANY);
  si_me.sin_port = htons(PORT);
  
  if(bind(fd, (struct sockaddr*)&si_me, sizeof(si_me))<0){
    perror("bind failed");
    return 0;
  }

  /* Init predef addr of AS */
  updatePredefAddrWithIpAndPort(PREDEF_AS_ADDR, AS_IP, AS_PORT);

  struct sockaddr_in si_remote;
  socklen_t addrlen = ADDR_SIZE;
  while(1){
    udp_payload_size = recvfrom(fd, udp_payload, BUFSIZE, 0, (struct sockaddr *)&si_remote, &addrlen);
    if(udp_payload_size>0){
      PRINT("received message: %s from %s:%d\n",udp_payload, inet_ntoa(si_remote.sin_addr), si_remote.sin_port);
      handleUdpPackage(udp_payload, udp_payload_size, (ADDR_TYPE *)(&si_remote));
    }
    sendUdpPackage(fd, send_buf, BUFSIZE);
  }
}
