#include "treestate.h"

#ifdef USE_STATE

STATE_TYPE expected_states[STATE_TABLE_LEN][STATE_VECTOR_LEN] = {0};

int incExpectedState(unsigned int state_row, unsigned int state_col, unsigned int inc_pre_state)
{
  if(state_row < STATE_TABLE_LEN  && state_col < STATE_VECTOR_LEN){
    if(expected_states[state_row][state_col]+1 < STATE_UPPER_BOUNDARY){
      /*!
       * If the to increased state +1 is still in upper boundary
       */
      expected_states[state_row][state_col]++;
      return SUCC;
    }else{
      if(!inc_pre_state || 0 == state_col){
      /*!
       * If increase the previous column state is not wished or
       * if the state is already in the first column
       */
        expected_states[state_row][state_col] = STATE_UPPER_BOUNDARY;
        return FAIL;
      } else if(SUCC == incExpectedState(state_row, state_col-1, inc_pre_state)){
        /*!
         * the previous column state is successfully increased,
         * the to increased state can be increased from zero in the future
         */
        expected_states[state_row][state_col] = 0;
        return SUCC;
      } else{
        expected_states[state_row][state_col] = STATE_UPPER_BOUNDARY;
      }
    }
  }
  return FAIL;
}

#endif

