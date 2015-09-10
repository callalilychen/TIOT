#include "utils.h"

uint8_t * optimizedXOR(uint8_t * a, uint8_t * b, size_t len, uint8_t * res){
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
