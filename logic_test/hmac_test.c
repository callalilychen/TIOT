#include <string.h>
#include <openssl/sha.h>
#include <openssl/hmac.h>
#include <openssl/evp.h>
#include "treeconfig.h"
#include "utils.h"
#include "hmac.h"


int hmac_test(void)
{
  const char* secret = "test";
  unsigned char key[sha_construction.size*3];
  unsigned char key2[sha_construction.size*3];
  sha_construction.func((unsigned char*)secret, 4, key);
  SHA256((unsigned char*)secret, 4, key2);
  printBlock("key", key, sha_construction.size);
  printBlock("key2", key2,sha_construction.size);

  PRINT("TEST1 hash '%s': ", secret);
  if(memcmp(key, key2, sha_construction.size) ==0){
    PRINT("OK\n");
  }else{
    PRINT("FAILED\n");
  }

  const char* msg = "The quick brown fox jumps over the lazy dog";
  unsigned int len = sha_construction.size;
  unsigned char hm[len]; 
  unsigned char hm2[len]; 
  hmac(&sha_construction, key, sha_construction.size*3, (unsigned char*)msg, strlen(msg), hm, &len);
  printBlock("HMAC", hm, len);
  HMAC(EVP_sha256(), key, sha_construction.size*3, (unsigned char*)msg, strlen(msg), hm2, &len);
  printBlock("HMAC2", hm2, len);
  PRINT("length: %d\n", len);

  PRINT("TEST2 hmac '%s': ", msg);
  if(memcmp(hm, hm2, sha_construction.size) ==0){
    PRINT("OK\n");
  }else{
    PRINT("FAILED\n");
  }
  const char* k = "key";
  hmac(&sha_construction, k, strlen(k), (unsigned char*)msg, strlen(msg), hm, &len);
  printBlock("HMAC", hm, len);
  HMAC(EVP_sha256(), k, strlen(k), (unsigned char*)msg, strlen(msg), hm2, &len);
  printBlock("HMAC2", hm2, len);
  PRINT("length: %d\n", len);
  PRINT("TEST3 hmac '%s' with '%s' as key: ", msg, k);
  if(memcmp(hm, hm2, sha_construction.size) ==0){
    PRINT("OK\n");
  }else{
    PRINT("FAILED\n");
  }
  return 0;
}
