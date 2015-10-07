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
#include "applicationcmd.h"
#include "securitydescriptor.h"
#include "addr_descriptor.h"
#include "securitylayerhandler.h"
#include "tree.h"
#include "bitmap.h"

unsigned int handleUpdateAddr(unsigned char* req, unsigned int req_size, application_session * p_session);
const application updateaddrapplication = {
  .name = "addr:",
  .name_size = 5,
  .usage = "<id>:<ip>:<port>\tUpdate an address descriptor. [NOTE] ip: xxx.xxx.xxx.xxx",
  .required_right = NO_RIGHT,
  .func = handleUpdateAddr
};

/*!
 * \brief Handle request to update an address descriptor
 * 
 * \param req       Rest of request message after application name
 * \param req_size  Size of this message
 * \param p_session pointer to the corresponding application session
 *
 * \return          Response message size
 */
unsigned int handleUpdateAddr(unsigned char* req, unsigned int req_size, application_session * p_session){
  // XXX This application is only for ui, therefore, output is directly printed on the standard output via PRINT.
  unsigned int descriptor_id;
  char ip[16] = {0};
  unsigned int port;
  if(SSCAN((const char*)req, "%u:%s:%u", &descriptor_id, &ip, &port)==3){
    if(SUCC == updateAddrWithIpAndPort(descriptor_id, ip, port)){
      PRINT("%s update address:\n", SUCC_MESSAGE);
      printAddrDescriptor(descriptor_id);
      return 0;
    }
    if(descriptor_id < ADDR_DESCRIPTORS_LEN + ADDR_PREDEF_LEN){
      if(ADMIN_RIGHT == askForAdminRight()){
        if(SUCC == updatePredefAddrWithIpAndPort(descriptor_id, ip, port)){
          PRINT("%s update address as admin:\n", SUCC_MESSAGE);
          printAddrDescriptor(descriptor_id);
          return 0;
        }
      }else{
        PRINT("%s no right to change predefined addresses!:\n", ERROR_MESSAGE);

      }
    }  
  }
  PRINT("%s %s%s\n", USAGE_MESSAGE, updateaddrapplication.name, updateaddrapplication.usage);
  return 0;
}

unsigned int handleAddAddr(unsigned char* req, unsigned int req_size, application_session * p_session);
const application addaddrapplication = {
  .name = "addaddr:",
  .name_size = 8,
  .usage = "<ip>:<port>\tAdd an address descriptor. [NOTE] ip:xxx.xxx.xxx.xxx",
  .required_right = NO_RIGHT,
  .func = handleAddAddr
};

/*!
 * \brief Handle request to add an address descriptor
 * 
 * \param req       Rest of request message after application name
 * \param req_size  Size of this message
 * \param p_session pointer to the corresponding application session
 *
 * \return          Response message size
 */
unsigned int handleAddAddr(unsigned char* req, unsigned int req_size, application_session * p_session){
  // XXX This application is only for ui, therefore, output is directly printed on the standard output via PRINT.
  char ip[16] = {0};
  unsigned int port;
  if(SSCAN((const char*)req, "%s:%u", &ip, &port)==2){
    unsigned int descriptor_id = getLeastActiveAddrDescriptor();
    if(SUCC == updateAddrWithIpAndPort(descriptor_id, ip, port)){
      PRINT("%s add address:\n", SUCC_MESSAGE);
      printAddrDescriptor(descriptor_id);
    }
    return 0;
  }
  PRINT("%s %s%s\n", USAGE_MESSAGE, addaddrapplication.name, addaddrapplication.usage);
  return 0;
}

/*!
 * \brief Handle request to list all address descriptors 
 * 
 * \param req       Rest of request message after application name
 * \param req_size  Size of this message
 * \param p_session pointer to the corresponding application session
 *
 * \return          Response message size
 */
unsigned int handleLsAddr(unsigned char* req, unsigned int req_size, application_session * p_session){
  // XXX This application is only for ui, therefore, output is directly printed on the standard output via PRINT.
  printAllAddrDescriptors();
  return 0;
}

const application lsaddrapplication = {
  .name = "lsaddr",
  .name_size = 6,
  .usage = "\t\tList all address descriptors",
  .required_right = NO_RIGHT,
  .func = handleLsAddr
};

/*!
 * \}
 */
