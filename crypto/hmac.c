#include "hmac.h"
#include <stdint.h>

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

#define HMAC_BLOCK_LENGTH 64
void hmac(void (*hash)(unsigned char *, size_t, unsigned char *), size_t hash_block_len, unsigned char* key, size_t key_len, unsigned char* message,  size_t message_len, unsigned char* res){
    unsigned char tk[hash_block_len];
  if(key_len > HMAC_BLOCK_LENGTH){
    hash(key, key_len, tk);
    key = tk;
    key_len = hash_block_len;
  }
  unsigned char ik[HMAC_BLOCK_LENGTH+message_len];
  unsigned char ok[HMAC_BLOCK_LENGTH+hash_block_len];
  int start = 0;
  int end = key_len/WORD_SIZE;
  for(int i = start; i < end; i++){
     (WORD_TYPE ik)[i] = (WORD_TYPE key)[i] ^ IPAD;
     (WORD_TYPE ok)[i] = (WORD_TYPE key)[i] ^ OPAD;
  }
  if(end*WORD_SIZE < key_len){
    for(int i = end*WORD_SIZE; i< key_len; i++){
      ik[i] = key[i] ^ IPAD_SINGLE;
      ok[i] = key[i] ^ OPAD_SINGLE;
    }
    start = end+1;
    for(int i = key_len; i< start*WORD_SIZE; i++){
      ik[i] = IPAD_SINGLE;
      ok[i] = OPAD_SINGLE;
    }
  }else{
    start = end;
  }
  end = HMAC_BLOCK_LENGTH/WORD_SIZE;
  for(int i = start; i <end; i++){
     (WORD_TYPE ik)[i] = IPAD;
     (WORD_TYPE ok)[i] = OPAD;
  }
  memcpy(ik+HMAC_BLOCK_LENGTH, message, message_len);
  hash(ik, HMAC_BLOCK_LENGTH+message_len, res);
  memcpy(ok+HMAC_BLOCK_LENGTH, res, hash_block_len);
  hash(ok, HMAC_BLOCK_LENGTH + hash_block_len, res);
}
