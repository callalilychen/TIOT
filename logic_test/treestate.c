#include "treestate.h"

#include <string.h>

static STATE_TYPE states[MAX_CLIENTS] = {0};  

void resetState(){
  bzero(states, sizeof states);  
}

STATE_TYPE * getState(STATE_INDEX_TYPE index){
  if(index < MAX_CLIENTS)
    return &states[index];
  return NULL;
}

int setState(STATE_INDEX_TYPE index, STATE_INDEX_TYPE perm, STATE_INDEX_TYPE key){
  if(index < MAX_CLIENTS && perm >= states[index].perm_index && key >= states[index].key_index){
    states[index].perm_index = perm+1;
    states[index].key_index = key+1;

    return SUCC;
  }
  return FAIL;
}

int incPermIndex(STATE_INDEX_TYPE index){
  if(index < MAX_CLIENTS){
    if(states[index].perm_index == MAX_INDEX){
        return FAIL;
      }
    states[index].perm_index++;
  }
  return SUCC;
}
int incKeyIndex(STATE_INDEX_TYPE index){
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
