#ifndef __TREE_STATE_H__
#define __TREE_STATE_H__

#include <stdint.h>
#include "interface.h"

#ifdef  __cplusplus
extern "C" {
#endif

#define MAX_CLIENTS 10
#define STATE_INDEX_TYPE uint16_t
#define MAX_INDEX 0xffff
#define MAX_PERMISSION 0xff

typedef struct state_struct{
    STATE_INDEX_TYPE perm_index;
    STATE_INDEX_TYPE key_index;
  } state_struct;
  
#define STATE_TYPE state_struct
  
  void resetState();
  STATE_TYPE * getState(STATE_INDEX_TYPE);
  int setState(STATE_INDEX_TYPE, STATE_INDEX_TYPE, STATE_INDEX_TYPE);
  int incPermIndex(STATE_INDEX_TYPE index);
  int incKeyIndex(STATE_INDEX_TYPE index);

#ifdef  __cplusplus
}
#endif /* __cplusplus */

#endif /* __TREE_STATE_H__ */

