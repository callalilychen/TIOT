#include "testcase.h"

#include <stdio.h>
#include <string.h>

#include "treeconfig.h"
#include "treestate.h"

void printStateTable(unsigned int row_len, unsigned int col_len, char header[][20], STATE_TYPE table[][STATE_VECTOR_LEN]){
  PRINT("%s", (char *)(header[0]));
  for(int i = 1; i < col_len+1; i++){
    PRINT("\t%s", (char *)(header[i]));
  }
  PRINT("\n");
  for(int i = 0; i < row_len; i++){
    PRINT("%d\t", i);
    for(int ii = 0; ii < col_len; ii++){
      PRINT("\t%d", *(*(table+i)+ii));
    }
    PRINT("\n");
  }
}
int state_test(void){
  int error = 0;
  PRINT("TEST STATE:\n");
  /* XXX 
   * #define STATE_TABLE_LEN 0x4 
   * #define STATE_VECTOR_LEN 2  
   * #define STATE_SIZE 2        
   * #define STATE_TYPE uint16_t 
   * #define STATE_UPPER_BOUNDARY 0xff
   */
  char state_header[STATE_VECTOR_LEN+1][20] = {0};
  strcpy(state_header[0], (const char *)&"Start State");
  for(int i = 1; i < STATE_VECTOR_LEN+1; i++){
    sprintf(state_header[i], "c%d", i-1);
  }
  
  resetAllExpectedStates();
  printStateTable(STATE_TABLE_LEN, STATE_VECTOR_LEN, state_header, expected_states);

  unsigned incCount = 0xff;
  for(int i = 0; i < incCount; i++){
    incExpectedState(0, STATE_VECTOR_LEN - 1, 0);
  }
  STATE_TYPE expectedStateVector1[STATE_VECTOR_LEN] = {0, 0xff};
  STATE_TYPE * isStateVector = getExpectedStateVector(0);
  strcpy(state_header[0], (const char *)&"TC1 State");
  printStateTable(STATE_TABLE_LEN, STATE_VECTOR_LEN, state_header, expected_states);
  
  if(memcmp(expectedStateVector1, isStateVector, sizeof(expectedStateVector1))==0){
    PRINT("TESTCASE 1: OK\n");
  } else {
    PRINT("TESTCASE 1: FAILED\n");
    error++;
  }
  
  incExpectedState(0, STATE_VECTOR_LEN - 1, 1);
  STATE_TYPE expectedStateVector2[STATE_VECTOR_LEN] = {1, 0};
  isStateVector = getExpectedStateVector(0);
  strcpy(state_header[0], (const char *)&"TC1 State");
  printStateTable(STATE_TABLE_LEN, STATE_VECTOR_LEN, state_header, expected_states);
  
  if(memcmp(expectedStateVector2, isStateVector, sizeof(expectedStateVector2))==0){
    PRINT("TESTCASE 2: OK\n");
  } else {
    PRINT("TESTCASE 2: FAILED\n");
    error++;
  }

  incCount = 0xfffff;
  for(int i = 0; i < incCount; i++){
    incExpectedState(0, STATE_VECTOR_LEN - 1, 1);
  }
  STATE_TYPE expectedStateVector3[STATE_VECTOR_LEN] = {0xff, 0xff};
  isStateVector = getExpectedStateVector(0);
  strcpy(state_header[0], (const char *)&"TC3 State");
  printStateTable(STATE_TABLE_LEN, STATE_VECTOR_LEN, state_header, expected_states);
  
  if(memcmp(expectedStateVector3, isStateVector, sizeof(expectedStateVector3))==0){
    PRINT("TESTCASE 3: OK\n");
  } else {
    PRINT("TESTCASE 3: FAILED\n");
    error++;
  }
  
  updateExpectedState(0,0,1,1);
  isStateVector = getExpectedStateVector(0);
  strcpy(state_header[0], (const char *)&"TC3.1 State");
  printStateTable(STATE_TABLE_LEN, STATE_VECTOR_LEN, state_header, expected_states);
  
  if(memcmp(expectedStateVector3, isStateVector, sizeof(expectedStateVector3))==0){
    PRINT("TESTCASE 3.1: OK\n");
  } else {
    PRINT("TESTCASE 3.1: FAILED\n");
    error++;
  }
  
  setState(1,0,1);
  STATE_TYPE expectedStateVector4[STATE_VECTOR_LEN] = {1, 0};
  isStateVector = getExpectedStateVector(1);
  strcpy(state_header[0], (const char *)&"TC4 State");
  printStateTable(STATE_TABLE_LEN, STATE_VECTOR_LEN, state_header, expected_states);
  
  if(memcmp(expectedStateVector4, isStateVector, sizeof(expectedStateVector4))==0){
    PRINT("TESTCASE 4: OK\n");
  } else {
    PRINT("TESTCASE 4: FAILED\n");
    error++;
  }
  
  setState(1,1,1);
  STATE_TYPE expectedStateVector5[STATE_VECTOR_LEN] = {1, 1};
  isStateVector = getExpectedStateVector(1);
  strcpy(state_header[0], (const char *)&"TC5 State");
  printStateTable(STATE_TABLE_LEN, STATE_VECTOR_LEN, state_header, expected_states);
  
  if(memcmp(expectedStateVector5, isStateVector, sizeof(expectedStateVector5))==0){
    PRINT("TESTCASE 5: OK\n");
  } else {
    PRINT("TESTCASE 5: FAILED\n");
    error++;
  }
  
  updateExpectedState(1,1,0,1);
  isStateVector = getExpectedStateVector(1);
  strcpy(state_header[0], (const char *)&"TC5.1 State");
  printStateTable(STATE_TABLE_LEN, STATE_VECTOR_LEN, state_header, expected_states);
  
  if(memcmp(expectedStateVector5, isStateVector, sizeof(expectedStateVector5))==0){
    PRINT("TESTCASE 5.1: OK\n");
  } else {
    PRINT("TESTCASE 5.1: FAILED\n");
    error++;
  }
  
  updateExpectedState(1,1,1, 1);
  STATE_TYPE expectedStateVector6[STATE_VECTOR_LEN] = {1, 2};
  isStateVector = getExpectedStateVector(1);
  strcpy(state_header[0], (const char *)&"TC6 State");
  printStateTable(STATE_TABLE_LEN, STATE_VECTOR_LEN, state_header, expected_states);
  
  if(memcmp(expectedStateVector6, isStateVector, sizeof(expectedStateVector6))==0){
    PRINT("TESTCASE 6: OK\n");
  } else {
    PRINT("TESTCASE 6: FAILED\n");
    error++;
  }
  
  updateExpectedState(1,1,0xfe,1);
  STATE_TYPE expectedStateVector7[STATE_VECTOR_LEN] = {2, 0};
  isStateVector = getExpectedStateVector(1);
  strcpy(state_header[0], (const char *)&"TC7 State");
  printStateTable(STATE_TABLE_LEN, STATE_VECTOR_LEN, state_header, expected_states);
  
  if(memcmp(expectedStateVector7, isStateVector, sizeof(expectedStateVector7))==0){
    PRINT("TESTCASE 7: OK\n");
  } else {
    PRINT("TESTCASE 7: FAILED\n");
    error++;
  }
  
  STATE_TYPE testStates[STATE_VECTOR_LEN] = {1,0xff};
  updateExpectedStateVectorPartly(2, 0, testStates, 1, 1);
  STATE_TYPE expectedStateVector8[STATE_VECTOR_LEN] = {1, 1};
  isStateVector = getExpectedStateVector(2);
  strcpy(state_header[0], (const char *)&"TC8 State");
  printStateTable(STATE_TABLE_LEN, STATE_VECTOR_LEN, state_header, expected_states);
  
  if(memcmp(expectedStateVector8, isStateVector, sizeof(expectedStateVector8))==0){
    PRINT("TESTCASE 8: OK\n");
  } else {
    PRINT("TESTCASE 8: FAILED\n");
    error++;
  }  

  updateExpectedStateVectorPartly(2, 1, testStates,1, 1);
  STATE_TYPE expectedStateVector8_1[STATE_VECTOR_LEN] = {1, 2};
  isStateVector = getExpectedStateVector(2);
  strcpy(state_header[0], (const char *)&"TC8.1 State");
  printStateTable(STATE_TABLE_LEN, STATE_VECTOR_LEN, state_header, expected_states);
  
  if(memcmp(expectedStateVector8_1, isStateVector, sizeof(expectedStateVector8_1))==0){
    PRINT("TESTCASE 8.1: OK\n");
  } else {
    PRINT("TESTCASE 8.1: FAILED\n");
    error++;
  }  

  updateExpectedStateVectorPartly(2, 1,testStates+1,1, 1);
  STATE_TYPE expectedStateVector8_2[STATE_VECTOR_LEN] = {2, 0};
  isStateVector = getExpectedStateVector(2);
  strcpy(state_header[0], (const char *)&"TC8.1 State");
  printStateTable(STATE_TABLE_LEN, STATE_VECTOR_LEN, state_header, expected_states);
  
  if(memcmp(expectedStateVector8_2, isStateVector, sizeof(expectedStateVector8_2))==0){
    PRINT("TESTCASE 8.2: OK\n");
  } else {
    PRINT("TESTCASE 8.2: FAILED\n");
    error++;
  }  

  updateExpectedStateVector(3,testStates,1);
  STATE_TYPE expectedStateVector9[STATE_VECTOR_LEN] = {2, 0};
  isStateVector = getExpectedStateVector(3);
  strcpy(state_header[0], (const char *)&"TC9 State");
  printStateTable(STATE_TABLE_LEN, STATE_VECTOR_LEN, state_header, expected_states);
  
  if(memcmp(expectedStateVector9, isStateVector, sizeof(expectedStateVector9))==0){
    PRINT("TESTCASE 9: OK\n");
  } else {
    PRINT("TESTCASE 9: FAILED\n");
    error++;
  }  
return error;
}
