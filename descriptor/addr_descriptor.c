/*-
 * addr_descriptor.h - IP address descriptor 
 *
 * Copyright 2015 Wenwen Chen
*/

/*!
 * \addtogroup    descriptor 
 * \{
 *
 * \file
 * \brief       Implementation of the ip Address descriptor functions
 *
 * \author      Wenwen Chen 
 */
#include "addr_descriptor.h"

  descriptor addr_descriptors[ADDR_DESCRIPTORS_LEN] = {0};

  ADDR_TYPE descriptor_addrs[ADDR_DESCRIPTORS_LEN+ADDR_PREDEF_LEN] = {0}; 

  int updateAddrDescriptor(unsigned int id, void * p_addr, unsigned int addr_size){
    if(id < ADDR_DESCRIPTORS_LEN && addr_size < ADDR_SIZE){
      memcpy(descriptor_addrs+id, p_addr, addr_size);
      activeDescriptor(addr_descriptors+id);
      return SUCC;
    }
    return FAIL;
  }
  
  int updatePredefAddrDescriptor(unsigned int id, void * p_addr, unsigned int addr_size){
    if(id >= ADDR_DESCRIPTORS_LEN && id < ADDR_DESCRIPTORS_LEN+ADDR_PREDEF_LEN && addr_size < ADDR_SIZE){
      memcpy(descriptor_addrs+id, p_addr, addr_size);
      return SUCC;
    }
    return FAIL;
  }

  int checkAddr(unsigned int id, void * p_addr, unsigned int addr_size){
    return 0==memcmp(descriptor_addrs+id, p_addr, addr_size);
  }

/*!
 * \}
 */
