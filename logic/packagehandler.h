/*-
 * applicationlayer.c - UDP package handler 
 *
 * Copyright 2005 Wenwen Chen
*/

/*!
 * \defgroup    handler Handler
 * \{
 *
 * \file
 * \brief       Header definitions for the udp package functions
 *
 * \author      Wenwen Chen 
 */
#ifndef __PACKAGE_HANDLER_H__
#define __PACKAGE_HANDLER_H__
#include "treeconfig.h"
#include "securitylayer.h" 
#include "applicationlayer.h" 
#include "applicationhandler.h"
#include "addr_descriptor.h"
#ifdef  __cplusplus
extern "C" {
#endif

#define ADDR_SIZE sizeof(ADDR_TYPE)
  //TODO TEST
  inline  void (__attribute__((always_inline))printBlockTEST)(char* name, unsigned char* block, size_t block_len){
  PRINT("%s:\n", name);
  for (int i=0; i< block_len; i++){
    PRINT("%x|", block[i]);
  }
  PRINT("\n");
}

inline void (__attribute__((always_inline))sendUdpPackage)(int fd, unsigned char* send_buf, unsigned int max_send_buf){
    unsigned int send_buf_size = 0;
  unsigned int application_layer_msg_size = 0;
  unsigned char * application_layer_msg = NULL;
  unsigned int application_session = NO_SESSION;
  unsigned int security_descriptor = NO_SESSION;
  while(NULL!=(application_layer_msg = generateApplicationLayer(&application_session, &application_layer_msg_size, &security_descriptor))){
    send_buf_size = generateSecurityLayerHeader(security_descriptor, send_buf, max_send_buf);
    memcpy(send_buf+send_buf_size, application_layer_msg, application_layer_msg_size);
    send_buf_size += (application_layer_msg_size + generateSecurityLayerMAC(security_descriptor, send_buf+send_buf_size, application_layer_msg_size, max_send_buf-send_buf_size));
    printBlockTEST("send", send_buf, send_buf_size);
    ADDR_SEND_TYPE * addr = (ADDR_SEND_TYPE *)getAddr(getApplicationSession(application_session)->addr_descriptor);
    if(NULL != addr){
      SENDTO_FUNC(fd, send_buf, send_buf_size, 0, addr, (ADDR_LEN_TYPE)ADDR_SIZE);
    }else{
      send_buf[send_buf_size] = 0;
      PRINT("NO Addr for \"%s\"\n", send_buf);
    }
    clearApplicationSession(application_session); 
    removeSecurityLayerDescriptor(security_descriptor); 
  } 
}

  inline  unsigned int (__attribute__((always_inline))generateUdpPackage)(unsigned char* send_buf, unsigned int max_send_buf){
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
  inline  void (__attribute__((always_inline))handleUdpPackage)(unsigned char* udp_payload, unsigned int udp_payload_size, ADDR_TYPE *p_addr){
  unsigned int header_size = 0;
  unsigned int security_descriptor = handleSecurityLayer(udp_payload, &udp_payload_size, &header_size);
  if(udp_payload_size > 0){
    unsigned int addr_descriptor = addAddrDescriptors(p_addr, ADDR_SIZE);
    if(NO_DESCRIPTOR == addr_descriptor){
      PRINT("[ERROR] Failed to add the addr descritptor!\n");
    }else if(NO_SESSION == handleApplicationLayer(udp_payload+header_size, udp_payload_size, security_descriptor, addr_descriptor)){
      udp_payload[udp_payload_size+header_size] = 0;
      PRINT("[WARN] No application session for message \"%s\"!\n", (const char *)(udp_payload+header_size));
    }
  }
  }
#if(UI_APPLICATION_COUNT>0)
  inline  void (__attribute__((always_inline))handleCmdPackage)(unsigned char* str, unsigned int str_size){
  if(str_size > 0){
    unsigned int session_id = createApplicationSession(NO_DESCRIPTOR, NO_DESCRIPTOR);
    if(NO_SESSION != session_id){
      handleApplication(str, str_size, session_id, ui_application);
    }else{
      PRINT("[ERROR] No session for the application!");
    }
  }
}
#endif
#ifdef  __cplusplus
}
#endif /* __cplusplus */

#endif /* __PACKAGE_HANDLER_H__ */
/*!
 * \}
 */
