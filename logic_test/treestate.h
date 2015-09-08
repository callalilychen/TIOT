#ifndef __TREE_STATE_H__
#define __TREE_STATE_H__

#include <stdint.h>
#include <strings.h>
#include "interface.h"

#ifdef  __cplusplus
extern "C" {
#endif

#define MAX_CLIENTS 0xff
#define STATE_INDEX_SIZE 2
#define STATE_INDEX_TYPE uint16_t
#define MAX_INDEX 0xffff
#define MAX_PERMISSION 0xff

#define STATE_TYPE state_struct

  // TODO als array
  typedef struct state_struct{
    STATE_INDEX_TYPE perm_index;
    STATE_INDEX_TYPE key_index;
  } state_struct;

  extern STATE_TYPE states[MAX_CLIENTS];
   
  inline void (__attribute__((always_inline))resetState)(void){
    bzero(states, sizeof states);  
  }

  inline STATE_TYPE * (__attribute__((always_inline))getState)(STATE_INDEX_TYPE index){
    if(index < MAX_CLIENTS)
      return &states[index];
    return NULL;
  }

  int setState(STATE_INDEX_TYPE, STATE_INDEX_TYPE, STATE_INDEX_TYPE, unsigned int*);



#ifdef  __cplusplus
}
#endif /* __cplusplus */

#endif /* __TREE_STATE_H__ */

