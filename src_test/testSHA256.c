#include "testcases.h"
#include "crypto/sha256.h"

unsigned char block[SHA256_DIGEST_LENGTH+1] = "testtesttesttesttesttesttest  !!";
static unsigned char hash[SHA256_DIGEST_LENGTH];

inline void setupSHA256(void){

}

inline void printSHA256(void){
 #if defined(DEBUG) || defined(INFO)
  int i;
  print("SHA256 Value of %s: \n\r", block);
  for (i=0; i< SHA256_DIGEST_LENGTH; i++){
    print("%x", hash[i]);
  }
  print("\n\r");
#endif
}

int testSHA256(int newValue){
  block[28] = getValidASCII((uint8_t)(newValue>>8));
  block[29] = getValidASCII((uint8_t)newValue);
#ifdef SIGNAL
  testSignalHigh();
#endif
  sha256(block, SHA256_DIGEST_LENGTH, hash);
#ifdef SIGNAL
  testSignalLow();
#endif
  printSHA256();
  return 1;
}

