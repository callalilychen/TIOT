/*-
 * until.h - Utils functions 
 *
 * Copyright 2015 Wenwen Chen
*/

/*!
 * \defgroup    untils Untils function 
 * \{
 *
 * \file
 * \brief       Header definitions for the security layer protocol type 0
 *
 * \author      Wenwen Chen 
 */
#ifndef __UTILS_H__
#define __UTILS_H__

#include <stdint.h>
#include <stdlib.h>
#include "treeconfig.h"

#ifdef  __cplusplus
extern "C" {
#endif

#ifndef WORD_SIZE
#define WORD_SIZE 2
#endif
#if(WORD_SIZE == 1)
#define WORD_TYPE (uint8_t *)
#define WORD_SHIFT_SCALAR 0
#elif(WORD_SIZE == 2)
#define WORD_TYPE (uint16_t *)
#define WORD_SHIFT_SCALAR 1
#elif(WORD_SIZE == 4)
#define WORD_TYPE (uint32_t *)
#define WORD_SHIFT_SCALAR 2
#else
#define WORD_TYPE (uint64_t *)
#define WORD_SHIFT_SCALAR 4
#endif

#define X(a, b, len, res) optimizedXOR(a, b, len, res) 



inline uint8_t * (__attribute__((always_inline))optimizedXOR)(uint8_t * a, uint8_t * b, size_t len, uint8_t * res){
  int index = 0;

  int end_index = len >> WORD_SHIFT_SCALAR;

  for(; index < end_index; index++){
    (WORD_TYPE res)[index] = (WORD_TYPE a)[index] ^ (WORD_TYPE b)[index];
  }

  index = (index << WORD_SHIFT_SCALAR);
  for(; index < len; index ++){
    res[index] = a[index] ^ b[index];
  }
  return res;
}

void printIPv4(char* name, uint32_t ip);
void printBlock(char* name, unsigned char* block, size_t block_len);
void debugIPv4(char* name, uint32_t ip);
void debugBlock(char* name, unsigned char* block, size_t block_len);

#ifdef  __cplusplus
}
#endif /* __cplusplus */

#endif /* __UTILS_H__ */

/*!
 * \}
 */

