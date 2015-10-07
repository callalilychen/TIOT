/*-
 * applicationcmd.h - Command appliction for authorization server 
 *
 * Copyright 2015 Wenwen Chen
*/

/*!
 * \addtogroup    as_application 
 * \{
 *
 * \file
 * \brief       Header definitions for the authorization server command applications 
 *
 * \author      Wenwen Chen 
 */
#include "applicationcmd.h"
#include "securitydescriptor.h"
#include "addr_descriptor.h"
#include "securitylayerhandler.h"
#include "tree.h"
//#include "treestate.h"
#include "bitmap.h"

const application updateaddrapplication = {
  .name = "addr:",
  .name_size = 5,
  .required_right = NO_RIGHT,
  .func = handleUpdateAddr
};

  unsigned int handleUpdateAddr(unsigned char* req, unsigned int req_size, application_session * p_session){
    // XXX This application is only for ui, therefore, output is directly printed on the standard output via PRINT.
    unsigned int descriptor_id;
    char ip[16] = {0};
    unsigned int port;
    if(SSCAN((const char*)req, "%u:%s:%u", &descriptor_id, &ip, &port)==3){
      if(SUCC == updateAddrWithIpAndPort(descriptor_id, ip, port)){
        PRINT("[SUCCESS] update address:\n");
        printAddrDescriptor(descriptor_id);
        return 0;
      }
      if(descriptor_id < ADDR_DESCRIPTORS_LEN + ADDR_PREDEF_LEN){
        if(ADMIN_RIGHT == askForAdminRight()){
          if(SUCC == updatePredefAddrWithIpAndPort(descriptor_id, ip, port)){
            PRINT("[SUCCESS] update address as admin:\n");
            printAddrDescriptor(descriptor_id);
            return 0;
          }
        }else{
          PRINT("[FAIL] no right to change predefined addresses!:\n");
          
        }
      }  
    }
    PRINT("[USAGE] addr:<id>:<ip>:<port>\n\tip: xxx.xxx.xxx.xxx\n");
    return 0;
  }

const application addaddrapplication = {
  .name = "addaddr:",
  .name_size = 8,
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
        PRINT("[SUCCESS] add address:\n");
        printAddrDescriptor(descriptor_id);
      }
      return 0;
    }
    PRINT("[USAGE] addaddr:<ip>:<port>\n\tip: xxx.xxx.xxx.xxx\n");
    return 0;
  }

const application lsaddrapplication = {
  .name = "lsaddr",
  .name_size = 6,
  .required_right = NO_RIGHT,
  .func = handleLsAddr
};

  unsigned int handleLsAddr(unsigned char* req, unsigned int req_size, application_session * p_session){
    // XXX This application is only for ui, therefore, output is directly printed on the standard output via PRINT.
    printAllAddrDescriptors();
    return 0;
  }

//TODO add set root cmd
const application keyapplication = {
  .name = "key:",
  .name_size = 4,
  .required_right = NO_RIGHT,
  .func = handleKey
};

  unsigned int handleKey(unsigned char* req, unsigned int req_size, application_session * p_session){
    if(req_size < 0 || p_session == NULL){
      return 0;
    }
    unsigned int key_index;
    if(SSCAN((const char*)req, "%u", &key_index)==1){
      /* Use the first active secret */
      unsigned int secret_index = getFirstSetBit();
      if(NO_BIT != secret_index){
        TREE_STATE_TYPE perm_index = getExpectedState(secret_index,0);
        
        setPermIndex(p_session->security_descriptor_id, perm_index);
        setSecretIndex(p_session->security_descriptor_id, secret_index);

        /* Calculate key */
        unsigned int depth = 2;
        tree_edge * edges = getEdges(depth);
        edges[0].func = edgeFunc;
        edges[0].params = getPermCode(p_session->security_descriptor_id, &(edges[0].params_size));

        edges[1].func = edgeFunc;
        edges[1].params = (unsigned char *)(&key_index);
        edges[1].params_size = TREE_STATE_SIZE;
        tree_node * p_key_node = NULL;

        if(p_session->addr_descriptor_id < ADDR_DESCRIPTORS_LEN){
          p_key_node = fillNodes(getPathFromCachedNodes(depth, p_session->addr_descriptor_id), edges, depth+1, 1);
        }else{
          p_key_node = fillNodes(getPathFromRoot(depth), edges, depth+1, 1);
        }
        // add security layer descriptor
        if(SUCC != updateSecurityWithKey(p_session->security_descriptor_id, p_key_node)){
          // TODO fix
          //p_session->security_descriptor_id = addSecurityDescriptor(p_key_node, NO_RIGHT, DEFAULT_PROTOCOL_TYPE);
        }
        
        setKeyIndex(p_session->security_descriptor_id, key_index);
        // FIXME necessary? or error msg
        if(p_session->addr_descriptor_id == NO_DESCRIPTOR){
          p_session->addr_descriptor_id = PREDEF_RS_ADDR;
        }
        memcpy(p_session->message, &"test", 4);  
        p_session->message_size = 4;
      }else{
        char * error_msg = "[ERROR] No Active Secret!";
        memcpy(p_session->message, error_msg, strlen(error_msg));  
        p_session->message_size = strlen(error_msg);
      }
      return p_session->message_size;
    }
    char * error_msg = "[USAGE] key:%u";
    memcpy(p_session->message, error_msg, strlen(error_msg));  
    p_session->message_size = strlen(error_msg);
    return p_session->message_size;
  }

/*!
 * \}
 */
