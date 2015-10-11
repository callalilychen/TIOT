/*-
 * treestate.c - A state manager for the tree data structure with static memeory allocation
 *
 * Copyright 2015 Wenwen Chen
*/
/*!
 * @addtogroup    tree_state
 * @{
 *
 * @file
 * @brief       Implementation of the tree state management functions
 *
 * @author      Wenwen Chen 
 */
#include "treestate.h"
TREE_STATE_TYPE expected_states[TREE_STATE_TABLE_LEN][TREE_STATE_VECTOR_LEN] = {0};

int incExpectedState(unsigned int state_row, unsigned int state_col, unsigned int inc_pre_state)
{
  if(state_row >= TREE_STATE_TABLE_LEN || state_col >= TREE_STATE_VECTOR_LEN){
    return FAIL;
  }
  for(int i =  state_col; i>=0; i--){
    if(expected_states[state_row][i]+1 < TREE_STATE_UPPER_BOUNDARY){
      /*!
       * If the to increased state +1 is still in upper boundary
       */
      expected_states[state_row][i]++;
      PRINT("%s New state: ", INFO_MESSAGE);
      for(int j = 0; j < TREE_STATE_VECTOR_LEN; j++){
        PRINT("%x\t", expected_states[state_row][j]);
      }
      PRINT("\n");
      return SUCC;
    }else if(!inc_pre_state){
      /*!
       * If increase the previous column state is not wished
       */
        expected_states[state_row][i] = TREE_STATE_UPPER_BOUNDARY;
        return FAIL;
    }else if( 0 == i){
      /*!
       * If the state is already in the first column
       */
        for(int ii = 0; ii<= state_col; ii++){
          expected_states[state_row][ii] = TREE_STATE_UPPER_BOUNDARY;
        }
        return FAIL;
    } else {
      /*!
       * Increase the previous column state and set this column to zero
       */
      expected_states[state_row][i] = 0;
    }
  }
  return FAIL;
}
/*!
 * @}
 */ 

