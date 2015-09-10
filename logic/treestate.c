#include "treestate.h"


STATE_TYPE expected_states[MAX_VALID_STATES][MAX_LEVEL-1] = {0};  

int incState(unsigned int states_index, unsigned int state_level, unsigned int inc_pre_state)
{
  if(states_index < MAX_VALID_STATES){
    if(expected_states[states_index][state_level] == MAX_STATE){
      if(!inc_pre_state || 0 == state_level){
        return FAIL;
      }
      if(SUCC == incState(states_index, state_level-1, inc_pre_state)){
        expected_states[states_index][state_level] = 0;
        return SUCC;
      }
    }else{
      expected_states[states_index][state_level]++;
      return SUCC;
    }
  }
  return FAIL;
}


