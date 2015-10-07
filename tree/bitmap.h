/*-
 * treestate.h - A static bit map
 *
 * Copyright 2015 Wenwen Chen
 */

/*!
 * \addtogroup  tree_state
 * \{
 *
 * \file
 * \brief       Header definitions for the bit map functions
 *
 * \author      Wenwen Chen 
 */

#ifndef __BIT_MAP_H__
#define __BIT_MAP_H__

#include <stdint.h>

#include "treeconfig.h"

#ifdef  __cplusplus
extern "C" {
#endif

#ifdef USE_BIT_MAP
#define NO_BIT 0xff
#if  NO_BIT < BIT_MAP_LEN
#error "NO_BIT should be not smaller then BIT_MAP_LEN, change NO_BIT in bitmap.h"
#endif
#if(BIT_MAP_LEN & 0x7)
#define BIT_MAP_SIZE BIT_MAP_LEN >>3   /*!< Size of bit map in bit */
#else
#define BIT_MAP_SIZE (BIT_MAP_LEN>>3) + 1
#endif

  /*!
   * \brief Bit map with predefined size by BIT_MAP_SIZE 
   *
   *        At the beginning, all states will be set to zero 
   */
  extern uint8_t bitmap[BIT_MAP_SIZE];

  /**
   * \brief Search the first set bit
   *
   * \return the bit index
   */
  inline unsigned int __attribute__((always_inline))getFirstSetBit(void){
    for(int i =0; i < BIT_MAP_SIZE; i++){
      if(bitmap[i]!=0){
        for(int ii=0; ii<8; ii++){
          if(bitmap[i]&(1<<ii)){
            return ii+i*8;
          }
        }  
      }
    }
    return NO_BIT;
  }

  /*!
   * \brief Search the first not set bit
   *
   * \return the bit index
   */
  inline unsigned int __attribute__((always_inline))getFirstNotSetBit(void){
    PRINT("BIT_MAP_SIZE is %d\n", BIT_MAP_SIZE);
    for(int i =0; i < BIT_MAP_SIZE; i++){
      if(bitmap[i]!=0xff){
        for(int ii=0; ii<8; ii++){
          if(0==(bitmap[i]&(1<<ii))){
            return ii+i*8;
          }
        }  
      }
    }
    return NO_BIT;
  }

  /*!
   * \brief Clear all bits in the bit map (set to 0)
   *
   * \return         None
   */
  inline void __attribute__((always_inline))clearAllBits(void){
    for(int i=0; i < BIT_MAP_SIZE; i++){
      bitmap[i] = 0;
    }
  }

  /*!
   * \brief Set all bits in the bit map to 1
   *
   * \return         None
   */
  inline void __attribute__((always_inline))setAllBits(void){
    for(int i=0; i < BIT_MAP_SIZE; i++){
      bitmap[i] = 0xff;
    }
  }

  /*!
   * \brief Set the bit at the given index to 1
   *
   * \param   index  Index of to be set bit 
   *
   * \return         On success SUCC is returned, otherwise FAIL.
   */
  inline int __attribute__((always_inline))setBit(unsigned int index){
    if(index>>3 < BIT_MAP_SIZE){
      bitmap[index>>3] |= 1<<(index-((index>>3)<<3));
      return SUCC;
    }
    return FAIL;
  }

  /*!
   * \brief Clear the bit at the given index (set to 0)
   *
   * \param   index  Index of to be cleared bit 
   *
   * \return         On success SUCC is returned, otherwise FAIL.
   */
  inline int __attribute__((always_inline))clearBit(unsigned int index){
    if(index>>3 < BIT_MAP_SIZE){
      bitmap[index>>3] &= ~(1<<(index-((index>>3)<<3)));  
      return SUCC;
    }
    return FAIL;
  }

  /*!
   * \brief Get the bit at the given index (set to 0)
   *
   * \param   index  Index of the bit 
   *
   * \return         The bit at the given index or NO_BIT, if index is out of range
   */
  inline uint8_t __attribute__((always_inline))getBit(unsigned int index){
    if(index>>3 < BIT_MAP_SIZE){
      return bitmap[index>>3] & (1<<(index-((index>>3)<<3)));
    }
    return NO_BIT;
  }

  /*!
   * \brief Set the byte at the given index to 1
   *
   * \param   index  Index of to be set byte
   *
   * \return         On success SUCC is returned, otherwise FAIL.
   */
  inline int __attribute__((always_inline))setByte(unsigned int index){
    if(index < BIT_MAP_SIZE){
      bitmap[index] = 0xff;
      return SUCC;
    }
    return FAIL;
  }

  /*!
   * \brief Clear the Byte at the given index (set to 0)
   *
   * \param   index  Index of to be cleared byte 
   *
   * \return         On success SUCC is returned, otherwise FAIL.
   */
  inline int __attribute__((always_inline))clearByte(unsigned int index){
    if(index < BIT_MAP_SIZE){
      bitmap[index] = 0; 
      return SUCC;
    }
    return FAIL;
  }

  /*!
   * \brief Get the byte at the given index (set to 0)
   *
   * \param   index  Index of the byte 
   *
   * \return         The byte at the given index or NO_BIT, if index is out of range
   */
  inline uint8_t __attribute__((always_inline))getByte(unsigned int index){
    if(index < BIT_MAP_SIZE){
      return bitmap[index]; 
    }
    return NO_BIT;
  }

  /*!
   * \brief Set bytes from the given index
   *
   * \param   index  Index of the rist to be set byte 
   * \param   bytes  Pointer to the bytes values 
   * \param   size   Size of the bytes 
   *
   * \return         None
   */
  inline void __attribute__((always_inline))setBytes(unsigned int index, uint8_t* bytes, unsigned int size){
    for(int i=index; i<BIT_MAP_SIZE && i<size; i++){
      bitmap[i] = bytes[i];
    }
  }

#endif

#ifdef  __cplusplus
}
#endif /* __cplusplus */

#endif /* __BIT_MAP_H__ */
/*!
 * \} 
 */


