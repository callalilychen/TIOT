#include "hmac.h"
#include "treeconfig.h"

#define IPAD_SINGLE 0x36
#define OPAD_SINGLE 0x5c

#if(WORD_SIZE == 1)
#define IPAD 0x36
#define OPAD 0x5c
#elif(WORD_SIZE == 2)
#define IPAD 0x3636
#define OPAD 0x5c5c
#elif(WORD_SIZE == 4)
#define IPAD 0x36363636
#define OPAD 0x5c5c5c5c
#else
#define IPAD 0x3636363636363636
#define OPAD 0x5c5c5c5c5c5c5c5c
#endif

unsigned char* hmac(const struct hash_function_construction* hash, const void* key, int key_size, const unsigned char* message, int message_size, unsigned char* res, unsigned int * res_size){
    unsigned char tk[hash->size];
  if(key_size > hash->block_size){
    hash->func(key, (size_t)key_size, tk);
    key = tk;
    key_size = (int)(hash->size);
  }
  unsigned char ik[hash->block_size+message_size];
  unsigned char ok[hash->block_size+hash->size];
  int start = 0;
  int end = key_size/WORD_SIZE;
  for(int i = start; i < end; i++){
     (WORD_TYPE ik)[i] = (WORD_TYPE key)[i] ^ IPAD;
     (WORD_TYPE ok)[i] = (WORD_TYPE key)[i] ^ OPAD;
  }
  if(end*WORD_SIZE < key_size){
    for(int i = end*WORD_SIZE; i< key_size; i++){
      ik[i] = ((unsigned char*)key)[i] ^ IPAD_SINGLE;
      ok[i] = ((unsigned char*)key)[i] ^ OPAD_SINGLE;
    }
    start = end+1;
    for(int i = key_size; i< start*WORD_SIZE; i++){
      ik[i] = IPAD_SINGLE;
      ok[i] = OPAD_SINGLE;
    }
  }else{
    start = end;
  }
  end = hash->block_size/WORD_SIZE;
  for(int i = start; i <end; i++){
     (WORD_TYPE ik)[i] = IPAD;
     (WORD_TYPE ok)[i] = OPAD;
  }
  memcpy(ik+hash->block_size, message, (size_t)message_size);
  hash->func(ik, (size_t)(hash->block_size+message_size), res);
  memcpy(ok+hash->block_size, res, (size_t)(hash->size));
  hash->func(ok, (size_t)(hash->block_size + hash->size), res);

  *res_size = hash->size;
  return res;
}
