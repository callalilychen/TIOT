#include "utils.h"

void printIPv4(char* name, uint32_t ip){
  PRINT("%s: ", name);
  for(int i = 24; i > 0; i-=8){
    uint8_t t = (uint8_t)(ip >> i);
    PRINT("%d.", t);
  }
  uint8_t t = (uint8_t)ip;
  PRINT("%d", t);
}

void printBlock(char* name, unsigned char* block, size_t block_len){
  PRINT("%s: ", name);
  for (int i=0; i< block_len; i++){
    PRINT("%x|", block[i]);
  }
  PRINT("\n");
}
