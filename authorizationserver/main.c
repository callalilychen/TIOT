#include <string.h>
#include <openssl/sha.h>
#include <openssl/hmac.h>
#include <openssl/evp.h>
#include <openssl/md5.h>

#include <stdio.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <unistd.h>
#include <pthread.h>

#include "treestate.h"
#include "tree.h" 
#include "hmac.h"
#include "applicationexample.h"
#include "securitylayer.h" 
#include "applicationlayer.h" 
#include "utils.h"
#include "bitmap.h"

#define BUFSIZE 512
#define PORT 9000

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
  while(NULL!=(application_layer_msg = generateApplicationLayer(&application_session, &application_layer_msg_size, &security_descriptor))){
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
  int secret_index, perm_index;
  while(1){
    int recvlen = recvfrom(*(int*)p_fd, buf, BUFSIZE, 0, (struct sockaddr *)&si_remote, &addrlen);

    if(recvlen > 0 ){
      buf[recvlen] = 0;
      printf("received message: %s\n", buf);
      if(sscanf(buf, "%d:%d", &secret_index, &perm_index)==2){
        if(secret_index < MAX_VALID_STATES && perm_index < MAX_STATE){
          if(getBit(secret_index) && SUCC == clearBit(secret_index)){
            if(SUCC == setState(secret_index,0, perm_index,0, NULL)){
              unsigned char perm_code[2] = {(unsigned char)secret_index, (unsigned char)perm_index};
              tree_edge * edges = getEdges(1);
              edges[0].func = edgeFunc;

              edges[0].params = perm_code;
              edges[0].params_size = 2;
              tree_node * p_client_secret = fillNodes(getPathFromRoot(2), edges, 2, 1);
              printf("GET key\n");
              // FIXME do better
              int i = 0;
              for(; i < recvlen; i++){
                if(buf[i] == ':')
                  break;
              }
              buf[++i] = (uint8_t)secret_index;
              buf[++i] = (uint8_t)perm_index;
              buf[++i] = ':';
              memcpy(buf+i+1, p_client_secret->block, p_client_secret->size);
              sendto(*(int*)p_fd, buf, i+1+p_client_secret->size, 0, (struct sockaddr *)&si_remote, addrlen);
              printBlock("Sc", p_client_secret->block, p_client_secret->size);
            }else{
              setBit(secret_index);
            }
          }
        }
      }
      else if(sscanf(buf, "r%d", &secret_index)==1){
        if(SUCC == setBit(secret_index)){
          sprintf(buf, "Revoke %d. secret", secret_index);
          puts(buf);
          puts("\n");
          sendto(*(int*)p_fd, buf, strlen(buf), 0, (struct sockaddr *)&si_remote, addrlen);
        }
      }
    }
  }
  return NULL;
}

int main(int argc, char** argv)
{ 
  resetAllStates();
  setAllBits();

  const char root[5] = "test";
  setRoot((unsigned char *)root, 4);

  // Create a UDP socket and listen on a port
  struct sockaddr_in si_as;
  int fd;
  if((fd = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP))==-1){
    perror("Can not create socket\n");
    return 0;
  }
  
  si_as.sin_family = AF_INET;
  si_as.sin_addr.s_addr = htonl(INADDR_ANY);
  si_as.sin_port = htons(PORT);

  if(bind(fd, (struct sockaddr*)&si_as, sizeof(si_as))<0){
    perror("bind failed");
    return 0;
  }

  /* UDP Thread to handle received UDP messages*/
  pthread_t udp_thread;

  if(pthread_create(&udp_thread, NULL, &udpthread, &fd)){
    perror("pthread_create failed");
    return 0;
  }
  while(1){
    char str[100];
    scanf("%s",str);
    printf("%s\n",str);
    int secret_index, perm_index;
    if(sscanf(str, "%d,%d", &secret_index, &perm_index)==2){
      if(secret_index < MAX_VALID_STATES && perm_index < MAX_STATE){
        if(getBit(secret_index) && SUCC == clearBit(secret_index)){
          if(SUCC == setState(secret_index,0, perm_index,0, NULL)){
            unsigned char perm_code[2] = {(unsigned char)secret_index, (unsigned char)perm_index};
            tree_edge * edges = getEdges(1);
            edges[0].func = edgeFunc;

            edges[0].params = perm_code;
            edges[0].params_size = 2;
            tree_node * p_client_secret = fillNodes(getPathFromRoot(2), edges, 2, 1);
            printf("GET key\n");
          }else{
            setBit(secret_index);
          }
        }
      }
    }
    else if(sscanf(str, "r%d", &secret_index)==1){
      if(SUCC == setBit(secret_index)){
        printf("Revoke %d. secret\n", secret_index);
      }
    }
  }
  
  close(fd);
}
