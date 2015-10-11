/*-
 * applicationsecurity.c - Security application for authorization server 
 *
 * Copyright 2015 Wenwen Chen
 */

/*!
 * \addtogroup    as_application 
 * \{
 *
 * \file
 * \brief       Implementation of the authorization server security command applications 
 *
 * \author      Wenwen Chen 
 */
#include "applicationsecurity.h"
#include "applicationsend.h"
#include "securitydescriptor.h"
#include "securitylayerhandler.h"
#include "bitmap.h"


/*!
 * \brief Handle request to list all security descriptors 
 * 
 * \param req       Rest of request message after application name
 * \param req_size  Size of this message
 * \param p_session Pointer to the corresponding application session
 *
 * \return          Response message size
 */
unsigned int handleLsSecurity(unsigned char* req, unsigned int req_size, application_session * p_session);
const application lssecapplication = {
  .name = "sls",
  .name_size = 3,
  .usage = "\t\tList existing security descriptor informations",
  .required_right = NO_RIGHT,
  .func = handleLsSecurity
};
unsigned int handleLsSecurity(unsigned char* req, unsigned int req_size, application_session * p_session){
  // XXX This application is only for ui, therefore, output is directly printed on the standard output via PRINT.
  PRINT("  ");
  printSecurityDescriptorHeader();
  for(int i = 0; i < SECURITY_DESCRIPTORS_LEN + SECURITY_PREDEF_LEN; i++){
    if(i == selected_security_id){
      PRINT("* ");
    }else{
      PRINT("  ");
    }
    printSecurityDescriptor(i);
  }
  return 0;
}

/*!
 * \brief Handle request to add a new security descriptor 
 * 
 * \param req       Rest of request message after application name
 * \param req_size  Size of this message
 * \param p_session Pointer to the corresponding application session
 *
 * \return          Response message size
 */
unsigned int handleAddSec(unsigned char* req, unsigned int req_size, application_session * p_session);
const application addsecapplication = {
  .name = "sadd:",
  .name_size = 5,
  .usage = "%u:%u\t<type>:<perm>\tAdd a new permission",
  .required_right = NO_RIGHT,
  .func = handleAddSec
};
unsigned int handleAddSec(unsigned char* req, unsigned int req_size, application_session * p_session){
  // XXX This application is only for ui, therefore, output is directly printed on the standard output via PRINT.
  uint8_t type;
  RIGHT_TYPE perm;
  if(SSCAN((const char*)req, "%u:%u", &type, &perm)==2){
    /* Update the first active secret */
    unsigned int secret_index = getFirstNotSetBit();
    if(NO_BIT != secret_index){
      p_session->security_descriptor_id = getLeastActiveSecurityDescriptor();
      updateSecurityWithProtocolType(p_session->security_descriptor_id, type);
      TREE_STATE_TYPE perm_index = getExpectedState(secret_index,0);
      incExpectedState(secret_index, 0, 0);
      setBit(secret_index);
      setSecretIndex(p_session->security_descriptor_id, secret_index);
      setPermIndex(p_session->security_descriptor_id, perm_index);
      setPerm(p_session->security_descriptor_id, perm);
      setKeyIndex(p_session->security_descriptor_id, 0);
      PRINT("New Security descriptor:\n");
      printSecurityDescriptorHeader();
      printSecurityDescriptor(p_session->security_descriptor_id);
    } else{
      PRINT("%s No inactive Secret\n", ERROR_MESSAGE);
    }
  }else{
    PRINT("%s %s%s\n", USAGE_MESSAGE, addsecapplication.name, addsecapplication.usage);
  }
  return 0;
}

/*!
 * \brief Handle request to edit the protocol type of a security descriptor
 * 
 * \param req       Rest of request message after application name
 * \param req_size  Size of this message
 * \param p_session Pointer to the corresponding application session
 *
 * \return          Response message size
 */
unsigned int handleEditSecurityLayerProtocolType(unsigned char* req, unsigned int req_size, application_session * p_session);
const application editprotocoltypeapplication = {
  .name = "ptedit:",
  .name_size = 7,
  .usage = "%u:%u\t<id>:<type>\tEdit the protocol type of a security descriptor",
  .required_right = NO_RIGHT,
  .func = handleEditSecurityLayerProtocolType
};
unsigned int handleEditSecurityLayerProtocolType(unsigned char* req, unsigned int req_size, application_session * p_session){
  // XXX This application is only for ui, therefore, output is directly printed on the standard output via PRINT.
  unsigned int descriptor_id;
  uint8_t type;
  if(SSCAN((const char*)req, "%u:%u", &descriptor_id, &type)==2){
    if(descriptor_id < SECURITY_DESCRIPTORS_LEN + SECURITY_PREDEF_LEN){
      if(SUCC != updateSecurityWithProtocolType(descriptor_id, type)){
        if(ADMIN_RIGHT == askForAdminRight()){
          if(SUCC == updatePredefSecurityWithProtocolType(descriptor_id, type)){
            PRINT("%s Update security descriptor as admin:\n", SUCC_MESSAGE);
          }
        }else{
          PRINT("%s No right to change predefined security descriptor!:\n", ERROR_MESSAGE);
          return 0;
        }
      }
      printSecurityDescriptorHeader();
      printSecurityDescriptor(descriptor_id);
    }else{  
      PRINT("%s Descriptor id %u or protocol type %u is not valid!:\n", ERROR_MESSAGE, descriptor_id, type);
    }
  }else{
    PRINT("%s %s%s\n", USAGE_MESSAGE, editprotocoltypeapplication.name, editprotocoltypeapplication.usage);
  }
  return 0;
}

/*!
 * \brief Handle request to edit the secret index of a security descriptor
 * 
 * \param req       Rest of request message after application name
 * \param req_size  Size of this message
 * \param p_session Pointer to the corresponding application session
 *
 * \return          Response message size
 */
unsigned int handleEditSecretIndex(unsigned char* req, unsigned int req_size, application_session * p_session);
const application editsecretindexapplication = {
  .name = "sedit:",
  .name_size = 6,
  .usage = "%u:%u\t<id>:<index>\tEdit the security index of a security descriptor",
  .required_right = NO_RIGHT,
  .func = handleEditSecretIndex
};
unsigned int handleEditSecretIndex(unsigned char* req, unsigned int req_size, application_session * p_session){
  // XXX This application is only for ui, therefore, output is directly printed on the standard output via PRINT.
  unsigned int descriptor_id;
  TREE_STATE_TYPE secret_index;
  if(SSCAN((const char*)req, "%u:%u", &descriptor_id, &secret_index)==2){
    if(descriptor_id >= SECURITY_DESCRIPTORS_LEN + SECURITY_PREDEF_LEN){
      PRINT("%s Descriptor id %u is out of range!:\n", ERROR_MESSAGE, descriptor_id);
      return 0;
    }
    if(descriptor_id>=SECURITY_DESCRIPTORS_LEN){
      if(ADMIN_RIGHT == askForAdminRight()){
        PRINT("%s Update security descriptor as admin:\n", SUCC_MESSAGE);
      }else{
        PRINT("%s No right to change predefined security descriptor!:\n", ERROR_MESSAGE);
        return 0;
      }
    }
    if(0==getBit(secret_index)){
      TREE_STATE_TYPE old_secret_index = getSecretIndex(descriptor_id);
      if(SUCC == clearBit(old_secret_index) && SUCC == setBit(secret_index)){
        TREE_STATE_TYPE perm_index = getExpectedState(secret_index,0);
        incExpectedState(secret_index, 0, 0);
        setPermIndex(p_session->security_descriptor_id, perm_index);
        printSecurityDescriptorHeader();
        printSecurityDescriptor(descriptor_id);
      }
    }else{
      PRINT("%s Secret index %u is not valid!:\n", ERROR_MESSAGE, secret_index);
    }
  } else{
    PRINT("%s %s%s\n", USAGE_MESSAGE, editsecretindexapplication.name, editsecretindexapplication.usage);
  }
  return 0;
}

/*!
 * \brief Handle request to edit the permission of a security descriptor
 * 
 * \param req       Rest of request message after application name
 * \param req_size  Size of this message
 * \param p_session Pointer to the corresponding application session
 *
 * \return          Response message size
 */
      unsigned int handleEditPermission(unsigned char* req, unsigned int req_size, application_session * p_session);
      const application editpermapplication = {
        .name = "pedit:",
        .name_size = 6,
        .usage = "%u:%u\t<id>:<perm>\tEdit the permission right of a security descriptor",
        .required_right = NO_RIGHT,
        .func = handleEditPermission
      };
unsigned int handleEditPermission(unsigned char* req, unsigned int req_size, application_session * p_session){
  // XXX This application is only for ui, therefore, output is directly printed on the standard output via PRINT.
  unsigned int descriptor_id;
  RIGHT_TYPE perm;
  if(SSCAN((const char*)req, "%u:%u", &descriptor_id, &perm)==2){
    if(descriptor_id >= SECURITY_DESCRIPTORS_LEN + SECURITY_PREDEF_LEN){
      PRINT("%s Descriptor id %u is out of range!:\n", ERROR_MESSAGE, descriptor_id);
      return 0;
    }
    if(descriptor_id>=SECURITY_DESCRIPTORS_LEN){
      if(ADMIN_RIGHT == askForAdminRight()){
        PRINT("%s Update Security as admin:\n", SUCC_MESSAGE);
      }else{
        PRINT("%s No right to change predefined security descriptor!:\n", ERROR_MESSAGE);
        return 0;
      }
    }
    TREE_STATE_TYPE secret_index = getSecretIndex(descriptor_id);
    TREE_STATE_TYPE perm_index = getExpectedState(secret_index,0);
    incExpectedState(secret_index, 0, 0);
    setPermIndex(p_session->security_descriptor_id, perm_index);
    setPerm(p_session->security_descriptor_id, (RIGHT_TYPE)perm);
    printSecurityDescriptorHeader();
    printSecurityDescriptor(descriptor_id);
  } else{
    PRINT("%s %s%s\n", USAGE_MESSAGE, editpermapplication.name, editpermapplication.usage);
  }
  return 0;
}

/*!
 * \brief Handle request to edit the key index of a security descriptor
 * 
 * \param req       Rest of request message after application name
 * \param req_size  Size of this message
 * \param p_session Pointer to the corresponding application session
 *
 * \return          Response message size
 */
      unsigned int handleEditKey(unsigned char* req, unsigned int req_size, application_session * p_session);
      const application editkeyapplication = {
        .name = "kedit:",
        .name_size = 6,
        .usage = "%u:%u\t<id>:<index>\tEdit the key index of a security descriptor",
        .required_right = NO_RIGHT,
        .func = handleEditKey
      };
unsigned int handleEditKey(unsigned char* req, unsigned int req_size, application_session * p_session){
  // XXX This application is only for ui, therefore, output is directly printed on the standard output via PRINT.
  TREE_STATE_TYPE key_index;
  unsigned int descriptor_id;
  if(SSCAN((const char*)req, "%u:%u", &descriptor_id, &key_index)==1){
    if(descriptor_id >= SECURITY_DESCRIPTORS_LEN + SECURITY_PREDEF_LEN){
      PRINT("%s Descriptor id %u is out of range!:\n", ERROR_MESSAGE, descriptor_id);
      return 0;
    }
    if(descriptor_id>=SECURITY_DESCRIPTORS_LEN){
      if(ADMIN_RIGHT == askForAdminRight()){
        PRINT("%s Update Security as admin:\n", SUCC_MESSAGE);
      }else{
        PRINT("%s No right to change predefined security descriptor!:\n", ERROR_MESSAGE);
        return 0;
      }
    }
    p_session->security_descriptor_id = descriptor_id;
    setKeyIndex(p_session->security_descriptor_id, key_index-1);
    printSecurityDescriptorHeader();
    printSecurityDescriptor(descriptor_id);
  }else{
    PRINT("%s %s%s\n", USAGE_MESSAGE, editkeyapplication.name, editkeyapplication.usage);
  }
  return 0;
}

/*!
 * \brief Handle request to select an security descriptor as currently used security descriptor
 * 
 * \param req       Rest of request message after application name
 * \param req_size  Size of this message
 * \param p_session Pointer to the corresponding application session
 *
 * \return          Response message size
 */
        unsigned int handleSelectSecurity(unsigned char* req, unsigned int req_size, application_session * p_session);

        const application selectsecapplication = {
          .name = "ssel:",
          .name_size = 5,
          .usage = "%u\t<id>\tSelect an security descriptor",
          .required_right = NO_RIGHT,
          .func = handleSelectSecurity
        };
unsigned int handleSelectSecurity(unsigned char* req, unsigned int req_size, application_session * p_session){
  // XXX This application is only for ui, therefore, output is directly printed on the standard output via PRINT.
  unsigned int id;
  if(SSCAN((const char*)req, "%u", &id)==1){
    if(id < ADDR_DESCRIPTORS_LEN + ADDR_PREDEF_LEN){
      selected_security_id = id;
      PRINT("%s Select security descriptor:\n", SUCC_MESSAGE);
      printSecurityDescriptor(id);
    }else{
      PRINT("%s Descriptor id %u is out of range!:\n", ERROR_MESSAGE, id);
    }
  }else{
    PRINT("%s %s%s\n", USAGE_MESSAGE, selectsecapplication.name, selectsecapplication.usage);
  }
  return 0;
}
/*!
 * \}
 */
