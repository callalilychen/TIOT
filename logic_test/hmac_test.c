#include <string.h>
#include <openssl/sha.h>
#include <openssl/hmac.h>
#include <openssl/evp.h>
#include "interface.h"

int hmac_test(void)
{
  char* secret = "test";
  unsigned char key[sha256_construction.size*3];
  unsigned char key2[sha256_construction.size*3];
  sha256_construction.func((unsigned char*)secret, 4, key);
  SHA256((unsigned char*)secret, 4, key2);
  printBlock("key", key, sha256_construction.size);
  printBlock("key2", key2,sha256_construction.size );

  char* msg = "The quick brown fox jumps over the lazy dog";
  unsigned int len = sha256_construction.size;
  unsigned char hm[len]; 
  unsigned char hm2[len]; 
  hmac(sha256_construction, key, sha256_construction.size*3, (unsigned char*)msg, strlen(msg), hm, &len);
  printBlock("HMAC", hm, len);
  HMAC(EVP_sha256(), key, sha256_construction.size*3, (unsigned char*)msg, strlen(msg), hm2, &len);
  printBlock("HMAC2", hm2, len);
  PRINT("length: %d\n", len);

  unsigned char* k = "key";
  hmac(sha256_construction, k, strlen(k), (unsigned char*)msg, strlen(msg), hm, &len);
  printBlock("HMAC", hm, len);
  HMAC(EVP_sha256(), k, strlen(k), (unsigned char*)msg, strlen(msg), hm2, &len);
  printBlock("HMAC2", hm2, len);
  PRINT("length: %d\n", len);
  return 0;
}
