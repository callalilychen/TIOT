/*-
 * securitydescriptor.h - Security descriptor 
 *
 * Copyright 2015 Wenwen Chen
 */

/*!
 * \addtogroup    descriptor 
 * \{
 *
 * \file
 * \brief       Implementation of the security descriptor functions
 *
 * \author      Wenwen Chen 
 */
#include "securitydescriptor.h"
#include "securitylayerhandler.h"

descriptor security_descriptors[SECURITY_DESCRIPTORS_LEN] = {0};

descriptor_security descriptor_securitys[SECURITY_DESCRIPTORS_LEN+SECURITY_PREDEF_LEN] = {0};

int updateSecurityDescriptor(unsigned int id, void * p_sec, unsigned int sec_size){
  if(id < SECURITY_DESCRIPTORS_LEN){
    memcpy(descriptor_securitys+id, p_sec, sec_size);
    activeDescriptor(security_descriptors + id);
    return SUCC;
  }
  return FAIL;
}

int updatePredefSecurityDescriptor(unsigned int id, void * p_sec, unsigned int sec_size){
  if(id >= SECURITY_DESCRIPTORS_LEN && id < SECURITY_DESCRIPTORS_LEN+SECURITY_PREDEF_LEN){
    memcpy(descriptor_securitys+id, p_sec, sec_size);
    return SUCC;
  }
  return FAIL;
}

int checkSecurity(unsigned int id, void * p_sec, unsigned int sec_size){
  if(sec_size != DESCRIPTOR_SECURITY_SIZE){
    return 0;
  }

  descriptor_security * p_new_descriptor_security = (descriptor_security *)p_sec;
  if(0!=compareTreeNode(&(descriptor_securitys[id].key), &(p_new_descriptor_security->key))){
    return 0;
  }
  if(descriptor_securitys[id].protocol_type != p_new_descriptor_security->protocol_type){
    return 0;
  }
  return 1;
}

void printSecurityDescriptorHeader(){
  PRINT("Index\tProtocol Type\t");
  printSecurityDescriptorSecurityLayerHeader();
  PRINT("\n");
}    

void printSecurityDescriptor(unsigned int id){
  PRINT("%2u\t", id);
  if(id < SECURITY_DESCRIPTORS_LEN && DESCRIPTOR_INACTIVE == security_descriptors[id]){
    PRINT("inactive\n");
  }else if(id < SECURITY_DESCRIPTORS_LEN + SECURITY_PREDEF_LEN){
    PRINT("%u\t\t", descriptor_securitys[id].protocol_type);
    printSecurityDescriptorSecurityLayer(id);
    PRINT("\n"); 
  }else{
    PRINT("%s\n", ERROR_MESSAGE);
  }
}

int copySecurityDescriptor(unsigned int dest_id, unsigned int src_id){
  if(SUCC!=updateSecurityWithProtocolType(dest_id, getDescriptorProtocolType(src_id))){
    if(SUCC!=updatePredefSecurityWithProtocolType(dest_id, getDescriptorProtocolType(src_id))){
      return FAIL;
    }
  }
  return copySecurityDescriptorSecurityLayer(dest_id, src_id);
}
/*!
 * \}
 */
