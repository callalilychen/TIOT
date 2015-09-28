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

#include "treestate.h"
#include "tree.h" 
#include "hmac.h"
#include "addr_descriptor.h"
#include "applicationexample.h"
#include "securitylayer.h" 
#include "applicationlayer.h" 
#include "utils.h"

#define BUFSIZE 512
#define PORT 9001
#define RS_PORT 9999
#define RS_IP "127.0.0.1"

static pthread_mutex_t lock;

unsigned char send_buf[BUFSIZE] = {0};
unsigned int send_buf_size = 0;

socklen_t addrlen = sizeof(ADDR_TYPE);

static void printBlock(char* name, unsigned char* block, size_t block_len){
  PRINT("%s:\n", name);
  for (int i=0; i< block_len; i++){
    PRINT("%x|", block[i]);
  }
  PRINT("\n");
}

static inline void sendUdpPackage(int fd){
  unsigned int application_layer_msg_size = 0;
  unsigned char * application_layer_msg = NULL;
  unsigned int application_session = NO_SESSION;
  unsigned int security_descriptor = NO_SESSION;
  pthread_mutex_lock(&lock);
  while(NULL!=(application_layer_msg = generateApplicationLayer(&application_session, &application_layer_msg_size, &security_descriptor))){
    send_buf_size = generateSecurityLayerHeader(security_descriptor, send_buf, BUFSIZE);
    memcpy(send_buf+send_buf_size, application_layer_msg, application_layer_msg_size);
    send_buf_size += (application_layer_msg_size + generateSecurityLayerMAC(security_descriptor, send_buf+send_buf_size, application_layer_msg_size, BUFSIZE-send_buf_size));
    printBlock("send", send_buf, send_buf_size);
    struct sockaddr * addr = (struct sockaddr *)getAddr(getApplicationSession(application_session)->addr_descriptor);
    if(NULL != addr){
      sendto(fd, send_buf, send_buf_size, 0, addr, addrlen);
    }else{
      send_buf[send_buf_size] = 0;
      PRINT("NO Addr for \"%s\"\n", send_buf);
    }
    clearApplicationSession(application_session); 
    removeSecurityLayerDescriptor(security_descriptor); 
  } 
  pthread_mutex_unlock(&lock);
}

static inline void handleUdpPackage(unsigned char* udp_payload, unsigned int udp_payload_size, ADDR_TYPE *p_addr){
  unsigned int header_size = 0;
  pthread_mutex_lock(&lock);
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
  pthread_mutex_unlock(&lock);
}

static void * recvUdpThread(void* p_fd){
  char buf[BUFSIZE];
  struct sockaddr_in si_remote;
  while(1){
    int recvlen = recvfrom(*(int*)p_fd, buf, BUFSIZE, 0, (struct sockaddr *)&si_remote, &addrlen);
    if(recvlen > 0 ){
      printBlock("recv", (unsigned char *)buf, recvlen);
      handleUdpPackage((unsigned char*)buf, recvlen, (ADDR_TYPE *)&si_remote);
      sendUdpPackage(* (int *)p_fd);
    }
  }
  return NULL;
}
#if(UI_APPLICATION_COUNT>0)
static inline void handleCmdPackage(unsigned char* str, unsigned int str_size){
  if(str_size > 0){
    pthread_mutex_lock(&lock);
    unsigned int session_id = createApplicationSession(NO_DESCRIPTOR, NO_DESCRIPTOR);
    if(NO_SESSION != session_id){
      handleApplication(str, str_size, session_id, ui_application);
    }else{
      PRINT("[ERROR] No session for the application!");
    }
    pthread_mutex_unlock(&lock);
  }
}
static void * cmdThread(void* p_fd){
  char str[BUFSIZE];

  while(1){
    SCAN("%s",str);
    PRINT("%s (%lu)\n",str, strlen(str));
    handleCmdPackage((unsigned char *)str, strlen(str));
    
    sendUdpPackage(*(int *)p_fd);
  }
  return NULL;
}
#endif

int main(int argc, char** argv)
{ 
  resetAllExpectedStates();
  initApplicationSession();
  initSecurityDescriptor();

  const char root[5] = "test";
  setRoot((unsigned char *)root, 4);

  // Create a UDP socket and listen on a port
  struct sockaddr_in si_me;
  int fd;
  if((fd = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP))==-1){
    perror("Can not create socket!\n");
    return 0;
  }
  
  si_me.sin_family = AF_INET;
  si_me.sin_addr.s_addr = htonl(INADDR_ANY);
  si_me.sin_port = htons(PORT);

  if(bind(fd, (struct sockaddr*)&si_me, sizeof(si_me))<0){
    perror("bind failed");
    return 0;
  }

  /* Init predef addr of RS */
  addr_descriptors[PREDEF_RS_ADDR].addr.sin_family = AF_INET;
  addr_descriptors[PREDEF_RS_ADDR].addr.sin_port = htons(RS_PORT);
  inet_aton(RS_IP, &(addr_descriptors[PREDEF_RS_ADDR].addr.sin_addr.s_addr));
  addr_descriptors[PREDEF_RS_ADDR].state = DESCRIPTOR_ACTIVE;

  pthread_mutex_init (&lock, NULL);
  /* UDP Thread to handle received UDP messages */
  pthread_t udp_thread;
  if(pthread_create(&udp_thread, NULL, &recvUdpThread, &fd)){
    perror("pthread_create for udp failed!\n");
    return 0;
  }

#if(UI_APPLICATION_COUNT>0)
  /* STDIN Thread to handle terminal cmds */
  pthread_t cmd_thread;
  if(pthread_create(&cmd_thread, NULL, &cmdThread, &fd)){
    perror("pthread_create for cmd failed!\n");
    return 0;
  }
#endif
  
  pthread_join (udp_thread, NULL);
#if(UI_APPLICATION_COUNT>0)
  pthread_join (cmd_thread, NULL);
#endif

  close(fd);
}
