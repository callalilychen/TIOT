/*-
 * applicationaddr.c - Address command appliction for authorization server 
 *
 * Copyright 2015 Wenwen Chen
 */

/*!
 * \addtogroup    as_application 
 * \{
 *
 * \file
 * \brief        Implementation of the authorization server address command applications 
 *
 * \author      Wenwen Chen 
 */
#include "applicationaddr.h"
#include "applicationsend.h"
#include "addr_descriptor.h"

/*!
 * \brief Handle request to list all address descriptors 
 * 
 * \param req       Rest of request message after application name
 * \param req_size  Size of this message
 * \param p_session pointer to the corresponding application session
 *
 * \return          Response message size
 */
unsigned int handleLsAddr(unsigned char* req, unsigned int req_size, application_session * p_session);
const application lsaddrapplication = {
  .name = "als",
  .name_size = 3,
  .usage = "\t\tList all address descriptors",
  .required_right = NO_RIGHT,
  .func = handleLsAddr
};
unsigned int handleLsAddr(unsigned char* req, unsigned int req_size, application_session * p_session){
  // XXX This application is only for ui, therefore, output is directly printed on the standard output via PRINT.
  for(int i = 0; i < ADDR_DESCRIPTORS_LEN + ADDR_PREDEF_LEN; i++){
    if(i == selected_addr_id){
      PRINT("* ");
    }else{
      PRINT("  ");
    }
    printAddrDescriptor(i);
  }
  return 0;
}

/*!
 * \brief Handle request to add an address descriptor
 * 
 * \param req       Rest of request message after application name
 * \param req_size  Size of this message
 * \param p_session Pointer to the corresponding application session
 *
 * \return          Response message size
 */
unsigned int handleAddAddr(unsigned char* req, unsigned int req_size, application_session * p_session);
const application addaddrapplication = {
  .name = "aadd:",
  .name_size = 5,
  .usage = "%s:%u\t<ip>:<port>\tAdd an address descriptor. [NOTE] ip:xxx.xxx.xxx.xxx",
  .required_right = NO_RIGHT,
  .func = handleAddAddr
};
unsigned int handleAddAddr(unsigned char* req, unsigned int req_size, application_session * p_session){
  // XXX This application is only for ui, therefore, output is directly printed on the standard output via PRINT.
  char ip[16] = {0};
  unsigned int port;
  if(SSCAN((const char*)req, "%s:%u", &ip, &port)==2){
    unsigned int descriptor_id = getLeastActiveAddrDescriptor();
    if(SUCC == updateAddrWithIpAndPort(descriptor_id, ip, port)){
      PRINT("%s add address:\n", SUCC_MESSAGE);
      printAddrDescriptor(descriptor_id);
    }else{
      PRINT("%s Update address %s failed\n", ERROR_MESSAGE, req);
    }
  }else{
    PRINT("%s %s%s\n", USAGE_MESSAGE, addaddrapplication.name, addaddrapplication.usage);
  }
  return 0;
}

/*!
 * \brief Handle request to edit an address descriptor
 * 
 * \param req       Rest of request message after application name
 * \param req_size  Size of this message
 * \param p_session Pointer to the corresponding application session
 *
 * \return          Response message size
 */
unsigned int handleEditAddr(unsigned char* req, unsigned int req_size, application_session * p_session);
const application editaddrapplication = {
  .name = "aedit:",
  .name_size = 6,
  .usage = "%u:%s:%u\t<id>:<ip>:<port>\tEdit an address descriptor. [NOTE] ip: xxx.xxx.xxx.xxx",
  .required_right = NO_RIGHT,
  .func = handleEditAddr
};
unsigned int handleEditAddr(unsigned char* req, unsigned int req_size, application_session * p_session){
  // XXX This application is only for ui, therefore, output is directly printed on the standard output via PRINT.
  unsigned int descriptor_id;
  char ip[16] = {0};
  unsigned int port;
  if(SSCAN((const char*)req, "%u:%s:%u", &descriptor_id, &ip, &port)==3){
    if(SUCC == updateAddrWithIpAndPort(descriptor_id, ip, port)){
      PRINT("%s Update address:\n", SUCC_MESSAGE);
      printAddrDescriptor(descriptor_id);
    }else if(descriptor_id > ADDR_DESCRIPTORS_LEN && descriptor_id < ADDR_DESCRIPTORS_LEN + ADDR_PREDEF_LEN){
      if(ADMIN_RIGHT == askForAdminRight()){
        if(SUCC == updatePredefAddrWithIpAndPort(descriptor_id, ip, port)){
          PRINT("%s Update address as admin:\n", SUCC_MESSAGE);
          printAddrDescriptor(descriptor_id);
        }
      }else{
        PRINT("%s No right to change predefined addresses!:\n", ERROR_MESSAGE);
      }
    }else{  
      PRINT("%s Descriptor id %u is out of range!:\n", ERROR_MESSAGE, descriptor_id);
    }
  }else{
    PRINT("%s %s%s\n", USAGE_MESSAGE, editaddrapplication.name, editaddrapplication.usage);
  }
  return 0;
}

/*!
 * \brief Handle request to select an address descriptor as currently used address
 * 
 * \param req       Rest of request message after application name
 * \param req_size  Size of this message
 * \param p_session Pointer to the corresponding application session
 *
 * \return          Response message size
 */
unsigned int handleSelectAddr(unsigned char* req, unsigned int req_size, application_session * p_session);

const application selectaddrapplication = {
  .name = "asel:",
  .name_size = 5,
  .usage = "%u\t<id>\tSelect an address descriptor",
  .required_right = NO_RIGHT,
  .func = handleSelectAddr
};
unsigned int handleSelectAddr(unsigned char* req, unsigned int req_size, application_session * p_session){
  // XXX This application is only for ui, therefore, output is directly printed on the standard output via PRINT.
  unsigned int id;
  if(SSCAN((const char*)req, "%u", &id)==1){
    if(id < ADDR_DESCRIPTORS_LEN + ADDR_PREDEF_LEN){
      selected_addr_id = id;
      PRINT("%s Select address:\n", SUCC_MESSAGE);
      printAddrDescriptor(id);
    }else{
      PRINT("%s Desccriptor id %u is out of range!:\n", ERROR_MESSAGE, id);
    }
  }else{
    PRINT("%s %s%s\n", USAGE_MESSAGE, selectaddrapplication.name, selectaddrapplication.usage);
  }
  return 0;
}
/*!
 * \}
 */
