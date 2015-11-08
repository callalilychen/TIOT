#include "testcases.h"
#include "sha256.h"

unsigned char block[2*SHA256_DIGEST_LENGTH+1] = "  !testtesttesttesttesttesttesttesttesttesttesttesttesttesttest!";

static unsigned char hash[SHA256_DIGEST_LENGTH];

inline static void printSHA256(void){
  int i;
  DEBUG("SHA256 Value of %s: \n\r", block);
  for (i=0; i< SHA256_DIGEST_LENGTH; i++){
    DEBUG("%x", hash[i]);
  }
  DEBUG("\n\r");
}

int testSHA256(int newValue){
#ifdef SIGNAL
  signalHigh();
#endif
#ifdef TESTLPM
  __delay_cycles(12500);
#endif
#ifdef SIGNAL
  signalLow();
#endif

  block[0] = getValidASCII((uint8_t)(newValue>>8));
  block[1] = getValidASCII((uint8_t)newValue);
#ifdef SIGNAL
  signalHigh();
#endif
  sha256(block, SHA256_DIGEST_LENGTH, hash);
#ifdef SIGNAL
  signalLow();
#endif
  printSHA256();
#ifdef SIGNAL
  signalHigh();
#endif
  sha256(block, SHA256_DIGEST_LENGTH+16, hash);
#ifdef SIGNAL
  signalLow();
#endif
  printSHA256();
#ifdef SIGNAL
  signalHigh();
#endif
  sha256(block, 2*SHA256_DIGEST_LENGTH, hash);
#ifdef SIGNAL
  signalLow();
#endif
  printSHA256();
  return 1;
}

