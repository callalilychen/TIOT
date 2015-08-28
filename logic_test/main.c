#include <string.h>
#include <openssl/sha.h>
#include <openssl/hmac.h>
#include <openssl/evp.h>
#include <openssl/md5.h>
#include "interface.h"

void printBlock(char* name, unsigned char* block, size_t block_len){
  PRINT("%s:\n", name);
  for (int i=0; i< block_len; i++){
    PRINT("%x|", block[i]);
  }
  PRINT("\n");
}
int main(int argc, char** argv)
{
  char* secret = "test";
  unsigned char key[HASH_BLOCK_LENGTH*3] = {0};
  unsigned char key2[HASH_BLOCK_LENGTH*3] = {0};
  HASH_FUNC((unsigned char*)secret, 4, key);
  SHA256((unsigned char*)secret, 4, key2);
  printBlock("key", key, HASH_BLOCK_LENGTH);
  printBlock("key2", key2, HASH_BLOCK_LENGTH);

  char* msg = "Hi There haha";
  unsigned int len = HASH_BLOCK_LENGTH;
  unsigned char hm[len]; 
  unsigned char hm2[len]; 
  hmac(hashfunction, HASH_BLOCK_LENGTH, key, HASH_BLOCK_LENGTH*3, (unsigned char*)msg, strlen(msg), hm);
  printBlock("HMAC", hm, len);
  HMAC(EVP_sha256(), key, HASH_BLOCK_LENGTH*3, (unsigned char*)msg, strlen(msg), hm2, &len);
  printBlock("HMAC2", hm2, len);
  PRINT("length: %d\n", len);
}
