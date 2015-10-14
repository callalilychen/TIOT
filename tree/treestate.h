/*-
 * treestate.h - A state manager for the tree data structure with static memeory allocation
 * 
 * Copyright 2015 Wenwen Chen
 */

/*!
 * @defgroup    tree_state Tree state 
 * @{
 *
 * @file
 * @brief       Header definitions for the tree state management functions
 *
 * @author      Wenwen Chen 
 */

#ifndef __TREE_STATE_H__
#define __TREE_STATE_H__

/*****************************************************************************/
/* Include files                                                             */
/*****************************************************************************/
#include <stdint.h>
#include <strings.h>
#include "treeconfig.h"

#ifdef  __cplusplus
extern "C" {
#endif
#define NO_TREE_STATE TREE_STATE_UPPER_BOUNDARY
#if NO_TREE_STATE < TREE_STATE_TABLE_LEN || NO_TREE_STATE < TREE_STATE_VECTOR_LEN
#error "NO_TREE_STATE should be not smaller then TREE_STATE_TABLE_LEN and TREE_STATE_VECTOR_LEN, change NO_BIT in treestate.h"
#endif

  /*!
   * \brief State table with to expected state
   *
   *        At the beginning, all states will be set to zero 
   */
  extern TREE_STATE_TYPE expected_states[TREE_STATE_TABLE_LEN][TREE_STATE_VECTOR_LEN];

  /*!
   * \brief Reset all expected states to zero
   *
   * \return         None
   */
  inline void __attribute__((always_inline))resetAllExpectedStates(void){
    for(int i = 0; i < TREE_STATE_TABLE_LEN; i++){
      for(int ii = 0; ii < TREE_HEIGTH; ii++){
        expected_states[i][ii]  = 0;
      }
    }
  }

  /*!
   * \brief Reset a expected state vector to zero
   *
   * \param index  index of to be reset state vector (row number) 
   *
   * \return         None
   */
  inline void __attribute__((always_inline))resetExpectedStateVector(unsigned int index){
    for(int ii = 0; ii < TREE_HEIGTH; ii++){
      expected_states[index][ii]  = 0;
    }
  }

  /*!
   * \brief check, whether a state is valid 
   *
   *        A state is valid, iff this state is bigger than the shall-state
   *
   * \param is            The state to be checked
   * \param shall         The shall-state
   *
   * \return               On success SUCC is returned, otherwise FAIL.
   */
  inline int __attribute__((always_inline))validState(TREE_STATE_TYPE is, TREE_STATE_TYPE shall)
  {
    if(is >= shall){
      return SUCC;
    }
    return FAIL;
  }

  /*!
   * \brief  Invalid a state vector of the given row
   *
   *         Set the last state cell of the state vector to TREE_STATE_UPPER_BOUNDARY
   *
   * \param state_row      Row number of this state
   *
   * \return               On success SUCC is returned, otherwise FAIL.
   */
  inline int __attribute__((always_inline))invalidStateVector(unsigned int state_row)
  {
    if(state_row >= TREE_STATE_TABLE_LEN){
      return FAIL;
    }
    expected_states[state_row][TREE_STATE_VECTOR_LEN-1] = TREE_STATE_UPPER_BOUNDARY;
    return SUCC;
  }
  /*!
   * \brief Increase the value of a state from the expected states
   *
   * \param state_row      Row number of this state
   * \param state_col      Column number of this state
   * \param inc_pre_state  Flag to indicate, whether increase the previous state (in the previous column), if the to increased state reach the upper boundary
   *
   * \return               On success SUCC is returned, otherwise FAIL.
   */
  int incExpectedState(unsigned int state_row, unsigned int state_col, unsigned int inc_pre_state);

  /*!
   * \brief get a state-vector from the expected states
   *
   * \param index       Index of wished state-vector (row number)
   *
   * \return            The index-th state-vector if exists, otherwise NULL
   */
  inline TREE_STATE_TYPE * __attribute__((always_inline))getExpectedStateVector(unsigned int index){
    if(index < TREE_STATE_TABLE_LEN)
      return expected_states[index];
    return NULL;
  }

  /*!
   * \brief get a state from the expected states
   *
   * \param state_row      Row number of this state
   * \param state_col      Column number of this state
   *
   * \return               The state_row-th state_col-th state if exists, otherwise NULL
   */
  inline TREE_STATE_TYPE __attribute__((always_inline))getExpectedState(unsigned int state_row, unsigned int state_col){
    if(state_row >= TREE_STATE_TABLE_LEN || state_col >= TREE_STATE_VECTOR_LEN)
      return TREE_STATE_UPPER_BOUNDARY;
    return expected_states[state_row][state_col];
  }

  /*!
   * \brief  Set a state vector of the expected states
   *
   * \param states_index        Index of to be modified state-vector (row number)
   * \param p_states            Pointer to the new state-vector
   *
   * \return                    On success SUCC is returned, otherwise FAIL.
   *
   * \note                      The length of states must be equal to TREE_STATE_VECTOR_LEN 
   */
  inline int __attribute__((always_inline))setStateVector(unsigned int states_index, TREE_STATE_TYPE * p_state_vector)
    //TODO 
  {
    if(states_index >= TREE_STATE_TABLE_LEN || p_state_vector== NULL){
      return FAIL;
    }
    for(int i = 0; i < TREE_STATE_VECTOR_LEN; i++){
      if(SUCC != validState(p_state_vector[i], expected_states[states_index][i])){
        return FAIL;
      }
      expected_states[states_index][i] = p_state_vector[i];
    }
    return SUCC;
  }

  /*!
   * \brief   Set a state vector of the expected states partly
   *
   *          Not a complete state vector will be set, but only a part of a state-vector
   *
   * \param state_row       Row number of this state
   * \param start_col       Start number of column to be set
   * \param p_states        Pointer to the new state-vector
   * \param states_len      Length of the new state-vector
   *
   * \return                On success SUCC is returned, otherwise FAIL.
   */
  inline int __attribute__((always_inline))setStateVectorPartly(unsigned int state_row, unsigned int start_col, TREE_STATE_TYPE * p_states, unsigned int states_len)
  {
    if(state_row >= TREE_STATE_TABLE_LEN || states_len >= TREE_STATE_VECTOR_LEN || p_states == NULL){
      return FAIL;
    }
    for(int i = 0; i < states_len; i++){
      if(SUCC != validState(p_states[i], expected_states[state_row][start_col+i])){
        return FAIL;
      }
      expected_states[state_row][start_col+i] = p_states[i];
    }
    return SUCC;
  }

  /*!
   * \brief  Set a state of the expected states
   *
   * \param state_row       Row number of this state
   * \param state_col       Column number of this state
   * \param state           New state value
   *
   * \return                On success SUCC is returned, otherwise FAIL.
   */
  inline int __attribute__((always_inline))setState(unsigned int state_row, unsigned int state_col, TREE_STATE_TYPE state)
  {
    if(state_row >= TREE_STATE_TABLE_LEN || state_col >= TREE_STATE_VECTOR_LEN){
      return FAIL;
    }
    if(SUCC != validState(state, expected_states[state_row][state_col])){
      return FAIL;
    }
    expected_states[state_row][state_col] = state;
    return SUCC;
  }

  /*!
   * \brief   Update a state vector of the expected states
   *
   *          Check the validation of the given state vector and set the state in the expected states table to the vector value 
   *          and then increase the last state of the vector, if it's valid
   *
   * \param states_index        Index of to be modified state-vector (row number)
   * \param p_states            Pointer to the new state-vector
   * \param inc_pre_state       Flag to indicate, whether increase the previous state (in the previous column), if the to increased state reach the upper boundary
   *
   * \return                    On success SUCC is returned, otherwise FAIL.
   *
   * \note                      The length of states must be equal to TREE_STATE_VECTOR_LEN 
   */
  inline int __attribute__((always_inline))updateExpectedStateVector(unsigned int state_index, TREE_STATE_TYPE * p_state_vector, unsigned int inc_pre_state)
  {
    if(SUCC == setStateVector(state_index, p_state_vector)){
      return incExpectedState(state_index, TREE_STATE_VECTOR_LEN-1, inc_pre_state);
    }
    return FAIL;
  }

  /*!
   * \brief   Update the state vector of the expected states partly
   *
   *          Check the validation of the given state vector and set the state in the expected states table to the vector value 
   *          and then increase the last state of the state-vector, if it's valid
   *
   * \param state_row       Row number of this state
   * \param start_col       Start number of column to be set
   * \param p_states        Pointer to the new state-vector
   * \param states_len      Length of the new state-vector
   * \param inc_pre_state   Flag to indicate, whether increase the previous state (in the previous column), if the to increased state reach the upper boundary
   *
   * \return                On success SUCC is returned, otherwise FAIL.
   */
  inline int __attribute__((always_inline))updateExpectedStateVectorPartly(unsigned int state_row, unsigned int start_col, TREE_STATE_TYPE * p_states, unsigned int states_len, unsigned int inc_pre_state)
  {
    if(SUCC == setStateVectorPartly(state_row, start_col, p_states, states_len)){
      return incExpectedState(state_row, TREE_STATE_VECTOR_LEN-1, inc_pre_state);
    }
    return FAIL;
  }

  /*!
   * \brief   Update a state of the expected states
   *
   *          Check the validation of the given state and set the state in the expected states table with its value plus 1, if it's valid
   *
   * \param state_row       Row number of this state
   * \param state_col       Column number of this state
   * \param state           New state value
   * \param inc_pre_state   Flag to indicate, whether increase the previous state (in the previous column), if the to increased state reach the upper boundary
   *
   * \return                On success SUCC is returned, otherwise FAIL.
   */
    inline int __attribute__((always_inline))updateExpectedState(unsigned int state_row, unsigned int state_col, TREE_STATE_TYPE state, unsigned int inc_pre_state)
    {
      if(SUCC == setState(state_row, state_col, state)){
        return incExpectedState(state_row, TREE_STATE_VECTOR_LEN-1, inc_pre_state);
      }
      return FAIL;
    }


#ifdef  __cplusplus
}
#endif /* __cplusplus */
#endif /* __TREE_STATE_H__ */
/*! 
 * @} 
 */

