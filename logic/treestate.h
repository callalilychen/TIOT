#ifndef __TREE_STATE_H__
#define __TREE_STATE_H__

#include <stdint.h>
#include <strings.h>
#include "treeconfig.h"

#ifdef  __cplusplus
extern "C" {
#endif

#ifdef USE_STATE

  /*!
   * \brief State table with to expected state
   *
   *        At the beginning, all states will be set to zero 
   *
   * \return         None
   */
  extern STATE_TYPE expected_states[STATE_TABLE_LEN][STATE_VECTOR_LEN];

  /*!
   * \brief Reset all states to zero
   *
   * \return         None
   */
  inline void (__attribute__((always_inline))resetAllStates)(void){
    bzero(expected_states, MAX_VALID_STATES*(MAX_LEVEL-1)*STATE_SIZE);  
  }

  /*!
   * \brief Reset a state vector to zero
   *
   * \param index  index of to be reset state vector (row number) 
   *
   * \return         None
   */
  inline void (__attribute__((always_inline))resetStateVector)(unsigned int index){
    bzero(expected_states+index, (MAX_LEVEL-1)*STATE_SIZE);  
  }

  /*!
   * \brief Increase the value of a state
   *
   * \param state_row      Row number of this state
   * \param state_col      Column number of this state
   * \param inc_pre_state  Flag to indicate, whether increase the previous state (in the previous column), if the to increased state reach the upper boundary
   *
   * \return               On success SUCC is returned, otherwise FAIL.
   */
  int incState(unsigned int state_row, unsigned int state_col, unsigned int inc_pre_state);

  inline STATE_TYPE * (__attribute__((always_inline))getStates)(unsigned int index){
    if(index < MAX_VALID_STATES)
      return expected_states[index];
    return NULL;
  }

  inline STATE_TYPE (__attribute__((always_inline))getState)(unsigned int index, unsigned int level){
    if(index >= MAX_VALID_STATES || level >= MAX_LEVEL-1)
      return NULL;
    return expected_states[index][level];
  }


  inline int (__attribute__((always_inline))validState)(STATE_TYPE is, STATE_TYPE shall)
  {
    if(is >= shall){
      return SUCC;
    }
    return FAIL;
  }
  // TODO remove first_state_changed, add states len
  /*!
   * \brief  Exemplarische Funktion
   *
   *         Diese Funktion gibt den übergebenen Parameter
   *         auf der Konsole aus.
   *
   * \param	parameter   Auszugebender Parameter
   * \return	            Status-Code
   *
   */
  inline int (__attribute__((always_inline))setStates)(unsigned int states_index, STATE_TYPE * states, unsigned int inc_pre_state)
  {
    if(states_index >= MAX_VALID_STATES || states == NULL){
      return FAIL;
    }
    if(first_state_changed!=NULL){
      *first_state_changed = (*states != *(expected_states[states_index]));
    }
    for(int i = 0; i < MAX_LEVEL - 1; i++){
      if(SUCC != validState(states[i], expected_states[states_index][i])){
        return FAIL;
      }
      expected_states[states_index][i] = states[i];
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
    if(first_state_changed!=NULL){
      *first_state_changed = (*states != *(expected_states[states_index]));
    }
    for(int i = 0; i < states_len; i++){
      if(SUCC != validState(states[i], expected_states[states_index][i])){
        return FAIL;
      }
      expected_states[states_index][i] = states[i];
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
    if(first_state_changed!=NULL){
      if(MAX_LEVEL-1==states_len){
        *first_state_changed = (*states != *(expected_states[states_index]));
      }else{
        *first_state_changed = 0;
      }
    }
    for(int i = MAX_LEVEL-1-states_len; i < MAX_LEVEL - 1; i++){
      if(SUCC != validState(states[i], expected_states[states_index][i])){
        return FAIL;
      }
      expected_states[states_index][i] = states[i];
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
    if(first_state_changed!=NULL){
      if(state_level==0){
        *first_state_changed = (state != *(expected_states[states_index]));
      }else{
        *first_state_changed = 0;
      }
    }
    if(SUCC != validState(state, expected_states[states_index][state_level])){
      return FAIL;
    }
    expected_states[states_index][state_level] = state;
    if(FAIL == incState(states_index, state_level, inc_pre_state)){
      return FAIL;
    }
    return SUCC;
  }

#endif

#ifdef  __cplusplus
}
#endif /* __cplusplus */

#endif /* __TREE_STATE_H__ */

