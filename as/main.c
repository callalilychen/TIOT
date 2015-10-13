#include <string.h>
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

#define PORT 9002
#define RS_PORT 5001  
#define CONSTRAINED_RS_IP "192.168.1.39"
#define RS_IP "127.0.0.1"

static pthread_mutex_t lock;

unsigned char main_buf[BUFSIZE+1] = {0};

socklen_t addrlen = sizeof(ADDR_TYPE);

static void * recvUdpThread(void* nothing){
  char buf[BUFSIZE];
  struct sockaddr_in si_remote;
  while(1){
    int recvlen = recvfrom(udp_socket_fd, buf, BUFSIZE, 0, (struct sockaddr *)&si_remote, &addrlen);
    if(recvlen > 0 ){
      pthread_mutex_lock(&lock);
      handleUdpPackage((unsigned char*)buf, recvlen, (ADDR_TYPE *)&si_remote);
      sendUdpPackage( main_buf, BUFSIZE);

      pthread_mutex_unlock(&lock);
    }
  }
  return NULL;
}

#if(UI_APPLICATION_COUNT>0)
static void * cmdThread(void* nothing){
  char str[BUFSIZE];

  while(1){
    SCAN("%s",str);
    PRINT("%s (%lu)\n",str, strlen(str));
    pthread_mutex_lock(&lock);
    handleCmdPackage((unsigned char *)str, strlen(str));
    sendUdpPackage(main_buf, BUFSIZE);
    pthread_mutex_unlock(&lock);
  }
  return NULL;
}

#endif
int main(int argc, char** argv)
{ 
  resetAllExpectedStates();
  resetApplicationSession();
  initApplication();

  const char root[5] = "test";
  setRoot((unsigned char *)root, 4);

  // Create a UDP socket and listen on a port
  struct sockaddr_in si_me;
  if((udp_socket_fd = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP))==-1){
    perror("Can not create socket!\n");
    return 0;
  }

  si_me.sin_family = ADDR_FAMILY;
  si_me.sin_addr.s_addr = htonl(INADDR_ANY);
  si_me.sin_port = htons(PORT);

  if(bind(udp_socket_fd, (struct sockaddr*)&si_me, sizeof(si_me))<0){
    perror("bind failed");
    return 0;
  }

  /* Init predef addr of RS */
  updateAddrWithIpAndPort(0, CONSTRAINED_RS_IP, RS_PORT);
  updatePredefAddrWithIpAndPort(PREDEF_RS_ADDR, RS_IP, RS_PORT);

  pthread_mutex_init (&lock, NULL);
  /* UDP Thread to handle received UDP messages */
  pthread_t udp_thread;
  if(pthread_create(&udp_thread, NULL, &recvUdpThread, NULL)){
    perror("pthread_create for udp failed!\n");
    return 0;
  }

#if(UI_APPLICATION_COUNT>0)
  /* STDIN Thread to handle terminal cmds */
  pthread_t cmd_thread;
  if(pthread_create(&cmd_thread, NULL, &cmdThread, NULL)){
    perror("pthread_create for cmd failed!\n");
    return 0;
  }
#endif

  pthread_join (udp_thread, NULL);
#if(UI_APPLICATION_COUNT>0)
  pthread_join (cmd_thread, NULL);
#endif

  close(udp_socket_fd);
  return 0;
}
