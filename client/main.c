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

#define BUFSIZE 512
#define PORT 9090
#define AS_PORT 9000
#define AS_IP "127.0.0.1"
//#define RS_PORT AS_PORT
#define RS_PORT 9999
#define RS_IP "127.0.0.1"

static inline void receivedUdpPackage(unsigned char* udp_payload, unsigned int udp_payload_size){
  unsigned int security_descriptor = 0;
  unsigned int header_size = 0;
  if(NO_DESCRIPTOR != (security_descriptor = handleSecurityLayer(udp_payload, &udp_payload_size, &header_size))){
    handleApplicationLayer(udp_payload+header_size, udp_payload_size, security_descriptor);
  }
}
unsigned char header[10] ={0};

static inline unsigned int sendUdpPackage(unsigned char* send_buf, unsigned int max_send_buf){
  unsigned int  application_layer_msg_size = 0;
  unsigned char *  application_layer_msg = NULL;
  unsigned int buf_size = 0;
  unsigned char *buf = send_buf;
  unsigned int application_session = NO_SESSION;
  unsigned int security_descriptor = NO_SESSION;
  while(NULL!=(application_layer_msg = generateApplicationLayer(&application_session, &application_layer_msg_size, &security_descriptor))){
    buf_size = 4;
    // FIXME generateSecurityLayerHeader(security_descriptor, buf, max_send_buf);
    memcpy(send_buf, header, 4);
    memcpy(send_buf+buf_size, application_layer_msg, application_layer_msg_size);
    buf_size +=application_layer_msg_size;
    buf_size +=generateSecurityLayerMAC(security_descriptor, application_layer_msg, application_layer_msg_size, send_buf+buf_size, max_send_buf-buf_size);
    clearApplicationLayerSession(application_session); 
    removeSecurityLayerDescriptor(security_descriptor); 
  } 
  return buf_size;
}

static void printBlock(char* name, unsigned char* block, size_t block_len){
  PRINT("%s:\n", name);
  for (int i=0; i< block_len; i++){
    PRINT("%x|", block[i]);
  }
  PRINT("\n");
}

static void * udpthread(void* p_fd){
  struct sockaddr_in si_remote;
  socklen_t addrlen = sizeof(si_remote);
  char buf[1+BUFSIZE];
  int rs_index;
  // TODO Storage
  unsigned char perm_code[10];
  while(1){
    int recvlen = recvfrom(*(int*)p_fd, buf, BUFSIZE, 0, (struct sockaddr *)&si_remote, &addrlen);
    if(recvlen > 0 ){
      buf[recvlen] = 0;
      printf("received message: %s from %s:%d\n", buf, inet_ntoa(si_remote.sin_addr), si_remote.sin_port);
      if(sscanf(buf, "%d:", &rs_index)==1){
        // FIXME do better
        int i = 0;
        for(; i < recvlen; i++){
          if(buf[i] == ':')
            break;
        }
        i++;
        int ii=0;
        for(; i < recvlen; i++){
          if(buf[i] == ':')
            break;
          perm_code[ii++] = buf[i];
        }
        printBlock("permcode", perm_code, ii);
        //TODO make header
        memcpy(header+1, perm_code, ii);
        i++;
        tree_node *root = getRoot();
        for(ii=0; i < recvlen && ii<HASH_SIZE; i++){
          root->block[ii++] = buf[i];
        }
        root->size = ii;
        printBlock("secret", root->block, ii);
        printf("F\n");
            
      }
      //sendto(*(int*)p_fd, buf, recvlen, 0, (struct sockaddr *)&si_remote, addrlen);
    }
  }
  return NULL;
}

  unsigned char send_buf[BUFSIZE] = {0};
  unsigned int send_buf_size = 0;
int main(int argc, char** argv)
{ 
  resetAllExpectedStates();

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

  /* UDP Thread to handle received UDP messages*/
  pthread_t udp_thread;
  if(pthread_create(&udp_thread, NULL, &udpthread, &fd)){
    perror("pthread_create failed");
    return 0;
  }

  struct sockaddr_in si_as, si_rs;
  si_as.sin_family = AF_INET;
  si_as.sin_port = htons(AS_PORT);
  inet_aton(AS_IP, &si_as.sin_addr.s_addr);
  si_rs.sin_family = AF_INET;
  si_rs.sin_port = htons(RS_PORT);
  inet_aton(AS_IP, &si_rs.sin_addr.s_addr);
  socklen_t addrlen = sizeof(si_as);

  while(1){
    // TODO in thread
    send_buf_size = sendUdpPackage(send_buf, BUFSIZE);
    if(send_buf_size >0){
      sendto(fd, send_buf, send_buf_size, 0, (struct sockaddr *)&si_rs, addrlen);
    }
    char str[100];
    scanf("%s",str);
    printf("%s (%lu)\n",str, strlen(str));
    int rs_index, perm_index;
    STATE_TYPE key_index;
    if(sscanf(str, "%d:%d", &rs_index, &perm_index)==2){
      printf("Send %s (%lu)\n",str, strlen(str));
      sendto(fd, str, strlen(str), 0, (struct sockaddr *)&si_as, addrlen);
    }
    else if(sscanf(str, "r%d", &rs_index)==1){
      printf("Send %s (%lu)\n",str, strlen(str));
      sendto(fd, str, strlen(str), 0, (struct sockaddr *)&si_as, addrlen);
    }
    else if(sscanf(str, "k%d", &key_index)==1){
      printf("Send %s (%lu)\n",str, strlen(str));
      //TODO generate version 0 protocol
      tree_edge * edges = getEdges(1);
      edges[0].func = edgeFunc;

      edges[0].params = (unsigned char *)(&key_index);
      edges[0].params_size = STATE_SIZE;
      // FIXME 
      header[3] = (uint8_t) key_index; 
      generateTest(addSecurityDescriptor(0,fillNodes(getPathFromRoot(2), edges, 2, 1)));
      
    }
    else if(memcmp(str, &"cmd", 3)){
      printf("Send %s (%lu)\n",str, strlen(str));
      //TODO get key index from state
    }
  }
  
  close(fd);
}
