/*-
 * descriptor.c - Descriptor 
 *
 * Copyright 2015 Wenwen Chen
*/

/*!
 * \addtogroup  descriptor
 * \{
 *
 * \file
 * \brief       Implementation the common descriptor functions
 *
 * \author      Wenwen Chen 
 */
#include "descriptor.h"

  unsigned int getLeastActiveDescriptor(descriptor *p_descriptors, unsigned int descriptors_len){
    unsigned int leastActiveDescriptorId=0; /*!<Store the id of the descriptor, which is the most inactive. That means, the descriptor value is the smallest*/
    uint8_t leastActiveDescriptorValue = DESCRIPTOR_MAX_COUNTER;
    for(int i=0; i< descriptors_len;i++){
      if(DESCRIPTOR_INACTIVE == p_descriptors[i]){
        return i;
      }
      if(p_descriptors[i] < leastActiveDescriptorValue){
        leastActiveDescriptorId = i;
        leastActiveDescriptorValue = p_descriptors[i];
      }
    }
    return leastActiveDescriptorId;
  }

  unsigned int addNewDescriptor(descriptor *p_descriptors, unsigned int descriptors_len, unsigned int predefined_data_len,  void * p_data, unsigned int len, 
      int (*checkFunc)(unsigned int, void *, unsigned int), int (*updateFunc)(unsigned int, void *, unsigned int)){
    unsigned int leastActiveDescriptorId=0; /*!<Store the id of the descriptor, which is the most inactive. That means, the descriptor value is the smallest*/
    uint8_t leastActiveDescriptorValue = DESCRIPTOR_MAX_COUNTER;
    int i = 0;
    for(; i< descriptors_len;i++){
      if(DESCRIPTOR_INACTIVE != p_descriptors[i]){
        if(checkFunc(i, p_data, len)){
          p_descriptors[i] = DESCRIPTOR_MAX_COUNTER;
          return DESCRIPTOR_MAX_COUNTER;
        }
        p_descriptors[i]--;
      }
      if(p_descriptors[i] < leastActiveDescriptorValue){
        leastActiveDescriptorId = i;
        leastActiveDescriptorValue = p_descriptors[i];
      }
    }
    for(i = 0; i< predefined_data_len; i++){
      if(checkFunc(descriptors_len+i, p_data, len)){
        return descriptors_len+i;
      } 
    }
    if(SUCC== updateFunc(leastActiveDescriptorId, p_data, len)){
      return leastActiveDescriptorId;
    }
    return NO_DESCRIPTOR;
  }
/*!
 * \}
 */
