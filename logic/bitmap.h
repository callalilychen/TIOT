#ifndef __BIT_MAP_H__
#define __BIT_MAP_H__

#include <stdint.h>
#include <strings.h>

#include "treeconfig.h"

#ifdef  __cplusplus
extern "C" {
#endif


  extern uint8_t bitmap [BIT_MAP_SIZE];
  
  inline void (__attribute__((always_inline))clearAllBits)(void){
    bzero(bitmap, BIT_MAP_SIZE);
  }

  inline void (__attribute__((always_inline))setAllBits)(void){
    for(int i=0; i < BIT_MAP_SIZE; i++){
      bitmap[i] = 0xff;
    }
  }
  
  inline int (__attribute__((always_inline))setBit)(unsigned int index){
    if(index>>3 < BIT_MAP_SIZE){
      bitmap[index>>3] |= 1<<(index-((index>>3)<<3));
      return SUCC;
    }
    return FAIL;
  }

  inline int (__attribute__((always_inline))clearBit)(unsigned int index){
    if(index>>3 < BIT_MAP_SIZE){
      bitmap[index>>3] &= ~(1<<(index-((index>>3)<<3)));  
      return SUCC;
    }
    return FAIL;
  }

  inline uint8_t (__attribute__((always_inline))getBit)(unsigned int index){
    if(index>>3 < BIT_MAP_SIZE){
      return bitmap[index>>3] & (1<<(index-((index>>3)<<3)));
    }
    return 0;
  }

  inline int (__attribute__((always_inline))setByte)(unsigned int index){
    if(index < BIT_MAP_SIZE){
      bitmap[index] = 0xff;
      return SUCC;
    }
    return FAIL;
  }

  inline int (__attribute__((always_inline))clearByte)(unsigned int index){
    if(index < BIT_MAP_SIZE){
      bitmap[index] = 0; 
      return SUCC;
    }
    return FAIL;
  }

  inline uint8_t (__attribute__((always_inline))getByte)(unsigned int index){
    if(index < BIT_MAP_SIZE){
      return bitmap[index]; 
    }
    return 0;
  }

  inline void (__attribute__((always_inline))setBits)(uint8_t* bytes, unsigned int size){
    for(int i=0; i<BIT_MAP_SIZE && i<size; i++){
      bitmap[i] = bytes[i];
    }
  }



#ifdef  __cplusplus
}
#endif /* __cplusplus */

#endif /* __BIT_MAP_H__ */

