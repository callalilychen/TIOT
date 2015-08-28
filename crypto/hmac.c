#include "hmac.h"

#include <openssl/md5.h>
#include <openssl/sha.h>


#define IPAD 0x36
#define OPAD 0x5c

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
  bzero(ik, HMAC_BLOCK_LENGTH+message_len);
  bzero(ok, 2*HMAC_BLOCK_LENGTH);
  for(int i = 0; i < key_len; i++){
    ik[i] = key[i] ^ IPAD;
    ok[i] = key[i] ^ OPAD;
  }
  for(int i = key_len; i < HMAC_BLOCK_LENGTH; i++){
    ik[i] = IPAD;
    ok[i] = OPAD;
  }
  memcpy(ik+HMAC_BLOCK_LENGTH, message, message_len);
  hash(ik, HMAC_BLOCK_LENGTH+message_len, res);
  memcpy(ok+HMAC_BLOCK_LENGTH, res, hash_block_len);
  hash(ok, HMAC_BLOCK_LENGTH + hash_block_len, res);
}
