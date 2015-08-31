#include <string.h>
#include <openssl/sha.h>
#include <openssl/hmac.h>
#include <openssl/evp.h>
#include "interface.h"

int hmac_test(void)
{
  char* secret = "test";
  unsigned char key[HASH_BLOCK_LENGTH*3] = {0};
  unsigned char key2[HASH_BLOCK_LENGTH*3] = {0};
  HASH_FUNC((unsigned char*)secret, 4, key);
  SHA256((unsigned char*)secret, 4, key2);
  printBlock("key", key, HASH_BLOCK_LENGTH);
  printBlock("key2", key2, HASH_BLOCK_LENGTH);

  char* msg = "The quick brown fox jumps over the lazy dog";
  unsigned int len = HASH_BLOCK_LENGTH;
  unsigned char hm[len]; 
  unsigned char hm2[len]; 
  hmac(hashfunction, HASH_BLOCK_LENGTH, key, HASH_BLOCK_LENGTH*3, (unsigned char*)msg, strlen(msg), hm);
  printBlock("HMAC", hm, len);
  HMAC(EVP_sha256(), key, HASH_BLOCK_LENGTH*3, (unsigned char*)msg, strlen(msg), hm2, &len);
  printBlock("HMAC2", hm2, len);
  PRINT("length: %d\n", len);

  unsigned char* k = "key";
  hmac(hashfunction, HASH_BLOCK_LENGTH, k, strlen(k), (unsigned char*)msg, strlen(msg), hm);
  printBlock("HMAC", hm, len);
  HMAC(EVP_sha256(), k, strlen(k), (unsigned char*)msg, strlen(msg), hm2, &len);
  printBlock("HMAC2", hm2, len);
  PRINT("length: %d\n", len);
  return 0;
}
