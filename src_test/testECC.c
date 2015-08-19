#include "testcases.h"
#include "uECC.h"

uint64_t g_rand = 88172645463325252ull;
uint8_t public[uECC_BYTES * 2];
uint8_t private[uECC_BYTES];
uint8_t hash[uECC_BYTES] = "testtesttesttest  !";
uint8_t sig[uECC_BYTES * 2];
uint8_t sign_state, verify_state;

int fake_rng(uint8_t *dest, unsigned size)
{
  while (size){
    g_rand ^= (g_rand << 13);
    g_rand ^= (g_rand >> 7);
    g_rand ^= (g_rand << 17);

    unsigned amount = (size > 8 ? 8 : size);
    memcpy(dest, &g_rand, amount);
    dest += amount;
    size -= amount;
  }
  return 1;
}

inline void setupECC()
{
#ifdef DEBUG
  print("Start ECC, Curve %d\n\r", uECC_curve());
  print("Start ECC, Wordsize %d\n\r", getWordSize());
  print("Start ECC, UECC Bytes %d\n\r", uECC_BYTES);
#endif
  uECC_set_rng(&fake_rng);
  int failed = uECC_make_key(public, private);
#ifdef INFO
  if (!failed){
    print("uECC_make_key() failed\n\r");
  }
#endif

#ifdef INFO
  int i;
  print("Public key:\n\r");
  for(i=0; i<uECC_BYTES * 2; i++){
    print("%x",public[i]);
  }
  print("\n\rPrivate key:\n\r");
  for(i=0; i<uECC_BYTES; i++){
    print("%x",private[i]);
  }
  print("\n\r");
#endif
  memcpy(hash, public, uECC_BYTES);
}

inline void updateECC(int newValue)
{
  sign_state = 0;
  verify_state = 0;
  //hash[16] = getValidASCII((uint8_t)(newValue>>8));
  //hash[17] = getValidASCII((uint8_t)newValue);

}

inline void printECC(void){
#ifdef INFO 
    int i=0;
  if(sign_state){
    print("Sign ");
    for(i=0; i<uECC_BYTES; i++){
    print("%x",hash[i]);
  }

    print(" success:\n\r");
    for (i=0;i<uECC_BYTES*2; i++){
      print("%x", sig[i]);  
    }
    print("\n\r");
    sign_state = 0;
  }else{
    if(verify_state){
      print("Verify success\n\r");
      
    }else{
      print("uECC failed\n\r");
    }
  }
#endif
}

inline int testECC(int newValue)
{
  updateECC(newValue);
#ifdef SIGNAL
  testSignalHigh();
#endif
  sign_state = uECC_sign(private, hash, sig);
#ifdef SIGNAL
  testSignalLow();
#ifdef TESTLPM
  __delay_cycles(1600);
#else
  __delay_cycles(8000000ul);
  __delay_cycles(8000000ul);
#endif
#endif
  printECC();
#ifdef SIGNAL
  testSignalHigh();
#endif
  verify_state = uECC_verify(public, hash, sig);
#ifdef SIGNAL
  testSignalLow();
#ifndef TESTLPM
  __delay_cycles(8000000ul);
  __delay_cycles(8000000ul);
  __delay_cycles(8000000ul);
  __delay_cycles(8000000ul);
#endif
#endif
  printECC();
  return sign_state|verify_state;

}
