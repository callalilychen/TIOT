#include "treestate.h"


STATE_TYPE states[MAX_CLIENTS] = {0};  

__attribute__((always_inline))
static inline int incPermIndex(STATE_INDEX_TYPE index)
{
  if(index < MAX_CLIENTS){
    if(states[index].perm_index == MAX_INDEX){
        return FAIL;
      }
    states[index].perm_index++;
  }
  return SUCC;
}

__attribute__((always_inline))
static inline int incKeyIndex(STATE_INDEX_TYPE index)
{
  if(index < MAX_CLIENTS){
    if(states[index].key_index == MAX_INDEX){
      if(states[index].perm_index == MAX_PERMISSION){
        return FAIL;
      }
      states[index].perm_index++;
    }
    states[index].key_index++;  
    return FAIL;
  }
  return SUCC;
}

int setState(STATE_INDEX_TYPE index, STATE_INDEX_TYPE perm, STATE_INDEX_TYPE key, unsigned int* flag){
  if(index < MAX_CLIENTS && perm >= states[index].perm_index && key >= states[index].key_index){
    *flag = (perm != states[index].perm_index);
    states[index].key_index = key;
    incKeyIndex(index);

    return SUCC;
  }
  return FAIL;
  if(index < MAX_CLIENTS && perm >= states[index].perm_index && key >= states[index].key_index){
    *flag = (perm != states[index].perm_index);
    states[index].key_index = key;
    incKeyIndex(index);

    return SUCC;
  }
  return FAIL;
}


