#include "utils.h"

void debugIPv4(char* name, uint32_t ip){
  DEBUG("%s: ", name);
  for(int i = 24; i > 0; i-=8){
    uint8_t t = (uint8_t)(ip >> i);
    DEBUG("%d.", t);
  }
  uint8_t t = (uint8_t)ip;
  DEBUG("%d", t);
}

void debugBlock(char* name, unsigned char* block, size_t block_len){
  DEBUG("%s: ", name);
  for (int i=0; i< block_len; i++){
    DEBUG("%x|", block[i]);
  }
  DEBUG("\n");
}
void printIPv4(char* name, uint32_t ip){
  VPRINT("%s: ", name);
  for(int i = 24; i > 0; i-=8){
    uint8_t t = (uint8_t)(ip >> i);
    VPRINT("%d.", t);
  }
  uint8_t t = (uint8_t)ip;
  VPRINT("%d", t);
}

void printBlock(char* name, unsigned char* block, size_t block_len){
  VPRINT("%s: ", name);
  for (int i=0; i< block_len; i++){
    VPRINT("%x|", block[i]);
  }
  VPRINT("\n");
}
