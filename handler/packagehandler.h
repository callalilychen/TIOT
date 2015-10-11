/*-
 * applicationlayer.c - UDP package handler 
 *
 * Copyright 2015 Wenwen Chen
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
#include "securitylayerhandler.h" 
#include "applicationlayerhandler.h" 
#include "applicationhandler.h"
#include "addr_descriptor.h"
#include "descriptor.h"
#include "utils.h"
#ifdef  __cplusplus
extern "C" {
#endif

#define ADDR_SIZE sizeof(ADDR_TYPE)

  inline void __attribute__((always_inline))sendUdpPackage(int fd, unsigned char* send_buf, unsigned int max_send_buf){
    for(int i = 0; i < APPLICATION_SESSIONS_LEN; i++){
      unsigned int send_buf_size = 0;
      unsigned int application_message_size = 0;
      unsigned int security_descriptor_id = NO_SESSION;
      unsigned int addr_descriptor_id = NO_SESSION;
      unsigned char * application_message = useApplicationSession(i, &application_message_size, &security_descriptor_id, &addr_descriptor_id);
      if(application_message!=NULL){
          ADDR_SEND_TYPE * p_addr = (ADDR_SEND_TYPE *)getAddr(addr_descriptor_id);
        if(NULL != p_addr){
          send_buf_size = generateSecurityLayerHeader(security_descriptor_id, send_buf, max_send_buf);
          unsigned int application_layer_msg_size = generateApplicationLayer(application_message, application_message_size, send_buf+send_buf_size, max_send_buf - send_buf_size);
          send_buf_size += (application_layer_msg_size + generateSecurityLayerMAC(security_descriptor_id, send_buf+send_buf_size, application_layer_msg_size, max_send_buf-send_buf_size));
          printBlock("send", send_buf, send_buf_size);
          
          printIPv4("to ",HTONL((((ADDR_TYPE*)p_addr)->sin_addr).s_addr));
          PRINT(":%u\n", HTONS(((ADDR_TYPE*)p_addr)->sin_port));
          SENDTO_FUNC(fd, send_buf, send_buf_size, 0, p_addr, (ADDR_LEN_TYPE)ADDR_SIZE);
        }else{
          send_buf[send_buf_size] = 0;
          PRINT("%s %s\n", INFO_MESSAGE, (const char*)application_message);
        }
        clearApplicationSession(i); 
      }
    } 
  }

  //  inline  unsigned int __attribute__((always_inline))generateUdpPackage(unsigned char* send_buf, unsigned int max_send_buf){
  //  unsigned int  application_layer_msg_size = 0;
  //  unsigned char *  application_layer_msg = NULL;
  //  unsigned int send_buf_size = 0;
  //  unsigned int application_session = NO_SESSION;
  //  unsigned int security_descriptor_id = NO_SESSION;
  //  while(NULL!=(application_layer_msg = generateApplicationLayer(&application_session, & application_layer_msg_size, &security_descriptor_id))){
  //    send_buf_size = 0;
  //    send_buf_size = generateSecurityLayerHeader(security_descriptor_id, send_buf, max_send_buf);
  //    memcpy(send_buf+send_buf_size, application_layer_msg, application_layer_msg_size);
  //    send_buf_size += (application_layer_msg_size+
  //        generateSecurityLayerMAC(security_descriptor_id, send_buf+send_buf_size, application_layer_msg_size, max_send_buf-send_buf_size));
  //    clearApplicationSession(application_session); 
  //    deactiveSecurityDescriptor(security_descriptor_id); 
  //  } 
  //  return send_buf_size;
  //}
  //
  inline  void __attribute__((always_inline))handleUdpPackage(unsigned char* udp_payload, unsigned int udp_payload_size, ADDR_TYPE *p_addr){
    unsigned int header_size = 0;
    udp_payload[udp_payload_size] = '\0';
    printBlock("received message", udp_payload, udp_payload_size);
    printIPv4("from ",HTONL(((p_addr)->sin_addr).s_addr));
    PRINT(":%u\n", HTONS(p_addr->sin_port));
    unsigned int security_descriptor_id = handleSecurityLayer(udp_payload, &udp_payload_size, &header_size);
    unsigned int addr_descriptor_id = addAddrDescriptor(p_addr, ADDR_SIZE);

    if(NO_DESCRIPTOR == addr_descriptor_id){
      PRINT("%s Failed to add the addr descritptor!\n", ERROR_MESSAGE);
    }else if(NO_SESSION == handleApplicationLayer(udp_payload+header_size, udp_payload_size, security_descriptor_id, addr_descriptor_id)){
      udp_payload[udp_payload_size+header_size] = 0;
      PRINT("[WARN] No application for \"%s\"!\n", (const char *)(udp_payload+header_size));
    }
  }

#if(UI_APPLICATION_COUNT>0)
  inline  void __attribute__((always_inline))handleCmdPackage(unsigned char* str, unsigned int str_size){
    if(str_size > 0){
      unsigned int session_id = createApplicationSession(NO_DESCRIPTOR, NO_DESCRIPTOR);
      if(NO_SESSION != session_id){
        handleApplication(str, str_size, session_id, ui_application);
      }else{
        PRINT("[WARN] No application for \"%s\"!\n", (const char *)str);
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
