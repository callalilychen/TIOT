#ifndef __TREE_STATE_H__
#define __TREE_STATE_H__

#include <stdint.h>
#include <strings.h>
#include "interface.h"

#ifdef  __cplusplus
extern "C" {
#endif

#define STATE_SIZE 2
#define STATE_TYPE uint16_t
#define MAX_STATE 0xffff
#define MAX_VALID_STATES MAX_CLIENTS 


  extern STATE_TYPE valid_states[MAX_VALID_STATES][MAX_LEVEL-1];

  inline void (__attribute__((always_inline))resetAllStates)(void){
    bzero(valid_states, MAX_VALID_STATES*(MAX_LEVEL-1)*STATE_SIZE);  
  }

  inline void (__attribute__((always_inline))resetStates)(unsigned int index){
    bzero(valid_states+index, (MAX_LEVEL-1)*STATE_SIZE);  
  }

  int incState(unsigned int, unsigned int, unsigned int);

  inline STATE_TYPE * (__attribute__((always_inline))getStates)(unsigned int index){
    if(index < MAX_VALID_STATES)
      return valid_states[index];
    return NULL;
  }

  inline STATE_TYPE (__attribute__((always_inline))getState)(unsigned int index, unsigned int level){
    if(index >= MAX_VALID_STATES || level >= MAX_LEVEL-1)
      return NULL;
    return valid_states[index][level];
  }


  inline int (__attribute__((always_inline))validState)(STATE_TYPE is, STATE_TYPE shall)
  {
    if(is >= shall){
      return SUCC;
    }
    return FAIL;
  }

  inline int (__attribute__((always_inline))setStates)(unsigned int states_index, STATE_TYPE * states, unsigned int inc_pre_state, unsigned int* first_state_changed)
  {
    if(states_index >= MAX_VALID_STATES || states == NULL){
      return FAIL;
    }
    *first_state_changed = (*states != *(valid_states[states_index]));
    for(int i = 0; i < MAX_LEVEL - 1; i++){
      if(SUCC != validState(states[i], valid_states[states_index][i])){
        return FAIL;
      }
      valid_states[states_index][i] = states[i];
      if(FAIL == incState(states_index, i, inc_pre_state)){
        return FAIL;
      }
    }
    return SUCC;
  }

  inline int (__attribute__((always_inline))setFirstStates)(unsigned int states_index, STATE_TYPE * states, unsigned int states_len, unsigned int inc_pre_state, unsigned int* first_state_changed)
  {
    if(states_index >= MAX_VALID_STATES || states_len >= MAX_LEVEL || states == NULL){
      return FAIL;
    }
    *first_state_changed = (*states != *(valid_states[states_index]));
    for(int i = 0; i < states_len; i++){
      if(SUCC != validState(states[i], valid_states[states_index][i])){
        return FAIL;
      }
      valid_states[states_index][i] = states[i];
      if(FAIL == incState(states_index, i, inc_pre_state)){
        return FAIL;
      }
    }
    return SUCC;
  }

  inline int (__attribute__((always_inline))setLastStates)(unsigned int states_index, STATE_TYPE * states, unsigned int states_len, unsigned int inc_pre_state, unsigned int* first_state_changed)
  {
    if(states_index >= MAX_VALID_STATES || states_len >= MAX_LEVEL || states == NULL){
      return FAIL;
    }
    if(MAX_LEVEL-1==states_len){
      *first_state_changed = (*states != *(valid_states[states_index]));
    }else{
      *first_state_changed = 0;
    }
    for(int i = MAX_LEVEL-1-states_len; i < MAX_LEVEL - 1; i++){
      if(SUCC != validState(states[i], valid_states[states_index][i])){
        return FAIL;
      }
      valid_states[states_index][i] = states[i];
      if(FAIL == incState(states_index, i, inc_pre_state)){
        return FAIL;
      }
    }
    return SUCC;
  }


  inline int (__attribute__((always_inline))setState)(unsigned int states_index, unsigned int state_level, STATE_TYPE state,  unsigned int inc_pre_state, unsigned int* first_state_changed)
  {
    if(states_index >= MAX_VALID_STATES || state_level >= MAX_LEVEL-1){
      return FAIL;
    }
    if(state_level==0){
      *first_state_changed = (state != *(valid_states[states_index]));
    }else{
      *first_state_changed = 0;
    }
    STATE_TYPE first_state = *(valid_states[states_index]);
    *first_state_changed = inc_pre_state;
    if(SUCC != validState(state, valid_states[states_index][state_level])){
      return FAIL;
    }
    valid_states[states_index][state_level] = state;
    if(FAIL == incState(states_index, state_level, inc_pre_state)){
      *first_state_changed = (first_state !=  *(valid_states[states_index])); 
      return FAIL;
    }
    *first_state_changed = (first_state !=  *(valid_states[states_index])); 
    return SUCC;
  }

#ifdef  __cplusplus
}
#endif /* __cplusplus */

#endif /* __TREE_STATE_H__ */

