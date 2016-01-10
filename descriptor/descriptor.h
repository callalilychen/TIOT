/*-
 * descriptor.h - Descriptor 
 *
   * Copyright 2015 Wenwen Chen
  */

  /*!
   * \defgroup    descriptor Descriptor
   * \brief An indictor used to store and access data or information
   * \{
   *
   * \file
   * \brief       Header definitions for the common descriptor functions
   *
   * \author      Wenwen Chen 
   */
#ifndef __DESCRIPTOR_H__
#define __DESCRIPTOR_H__

#include "treeconfig.h"
#include <stdint.h>
#include <stddef.h>
#include <string.h>

#ifdef  __cplusplus
extern "C" {
#endif

/*!
 * \brief Macro as indicator when no descriptor exists 
 */
#define NO_DESCRIPTOR 0xffff
/*!
 * \brief Macro as indicator when a descriptor is inactive 
 */
#define DESCRIPTOR_INACTIVE 0
/*!
 * \brief Macro as indicator when no descriptor state can be defind 
 */
#define DESCRIPTOR_NO_STATE 0xff
/*!
 * \brief Macro for the maximal allowed counter for dscriptor
 */
#define DESCRIPTOR_MAX_COUNTER 0xff


  /*!
   * \brief common descriptor
   * 
   * A descriptor is actually a count down counter to state, whether the descriptor for a corresponding data is active. If a descriptor become active, it will be assigned to the DESCRIPTOR_MAX_COUNTER. Everytime, if the descriptor is not to be used, descrease the value, until 0 is reached. This means the descriptor is inactive.
   */
  typedef uint8_t descriptor;

  /*!
   * \brief Active the given descriptor
   *
   * The given descriptor will be set to DESCRIPTOR_MAX_COUNTER, if the given pointer is not NULL
   *
   * \param p_descriptor  The pointer to the descriptor
   *
   * \return              NULL
   */
  inline void (__attribute__((always_inline))activeDescriptor)(descriptor* p_descriptor){
    if(p_descriptor!=NULL){
      *p_descriptor = DESCRIPTOR_MAX_COUNTER;
    }
  }
  
  /*!
   * \brief Deative the given descriptor
   *
   * The given descriptor will be set to DESCRIPTOR_INACTIVE, if the given pointer is not NULL
   *
   * \param p_descriptor  The pointer to the descriptor
   *
   * \return              The descriptor value
   */
  inline void (__attribute__((always_inline))deactiveDescriptor)(descriptor* p_descriptor){
    if(p_descriptor!=NULL)
      *p_descriptor = DESCRIPTOR_INACTIVE;
  }

  /*!
   * \brief Get the value of the given descriptor
   *
   * \param p_descriptor  The pointer to the descriptor
   *
   * \return              The value of the given descriptor
   */
  inline uint8_t (__attribute__((always_inline))getDescriptorValue)(descriptor* p_descriptor){
    if(p_descriptor!=NULL)
      return *p_descriptor;
  }

  /*!
   * \brief Initialize the given descriptor
   *      
   *        Set the state of the given descriptor to DESCRIPTOR_INACTIVE, if the given pointer is not NULL
   *
   * \param p_descriptor  The pointer to the descriptor
   *
   * \return              The descriptor value
   */
  inline void (__attribute__((always_inline))initDescriptor)(descriptor* p_descriptor){
    return deactiveDescriptor(p_descriptor); 
  }

   /*!
   * \brief Get the least active descriptor 
   *
   * \param p_descriptors         Pointer to the descriptors
   *        descriptors_len       Length of the descriptor
   *
   * \return                      The descriptor id of the least active descriptor 
   */
  unsigned int getLeastActiveDescriptor(descriptor *p_descriptors, unsigned int descriptors_len);
  
   /*!
   * \brief Store a new descriptor data 
   *
   *        If the new data is already existing in the store, return the corresponding descriptor id,
   *        Otherwise add the data to the position of the least active descriptor.
   *        
   * \param p_descriptors         Pointer to the descriptors
   *        descriptors_len       Length of the descriptor
   *        predefined_data_len   Length of the predefined data, which refer to no descriptor
   *        p_data                Pointer to the new data
   *        len                   Length of the new data
   *        checkFunc             Pointer to the function, which checks, whether the new value is already sored
   *        updateFunc            Pointer to the function, which update the corresponding descriptor data using the new data
   *
   * \return                      The descriptor id for the new given data
   */
  unsigned int addNewDescriptor(descriptor *p_descriptors, unsigned int descriptors_len, unsigned int predefined_data_len,  void * p_data, unsigned int len, int (*checkFunc)(unsigned int id, void *, unsigned int), int (*updateFunc)(unsigned int id, void *, unsigned int));
#ifdef  __cplusplus
}
#endif /* __cplusplus */

#endif /* __DESCRIPTOR_H__ */
/*!
 * \}
 */
