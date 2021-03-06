#include "testcases.h"
#include "uECC.h"

uint64_t g_rand = 88172645463325252ull;
uint8_t public[uECC_BYTES * 2];
uint8_t private[uECC_BYTES];
uint8_t hash[uECC_BYTES] = "  !testtesttesttest!";
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

void setupECC()
{
  DEBUG("Start ECC, Curve %d\n\r", uECC_curve());
  //  DEBUG("Start ECC, Wordsize %d\n\r", uECC_WORD_SIZE);
  DEBUG("Start ECC, UECC Bytes %d\n\r", uECC_BYTES);
  uECC_set_rng(&fake_rng);
  if (! uECC_make_key(public, private)){
    DEBUG("uECC_make_key() failed\n\r");
  }

  int i;
  DEBUG("Public key:\n\r");
  for(i=0; i<uECC_BYTES * 2; i++){
    DEBUG("%x",public[i]);
  }
  DEBUG("\n\rPrivate key:\n\r");
  for(i=0; i<uECC_BYTES; i++){
    DEBUG("%x",private[i]);
  }
  DEBUG("\n\r");
  //memcpy(hash, public, uECC_BYTES);
}

static inline void updateECC(int newValue)
{
  hash[0] = getValidASCII((uint8_t)(newValue>>8));
  hash[1] = getValidASCII((uint8_t)newValue);
  sign_state = 0;
  verify_state = 0;

}

static inline void printECC(void){
  int i=0;
  if(sign_state){
    DEBUG("Sign %s",hash);
    //for(i=0; i<uECC_BYTES; i++){
    //  DEBUG("%x",hash[i]);
    //}
    DEBUG(" success:\n\r");
    for (i=0;i<uECC_BYTES*2; i++){
      DEBUG("%x", sig[i]);  
    }
    DEBUG("\n\r");
  }
  if(verify_state){
    DEBUG("Verify success\n\r");
  }else{
    DEBUG("uECC failed\n\r");
  }
}

int testECC(int newValue)
{
#ifdef SIGNAL
  signalHigh();
#endif
#ifdef TESTLPM
  __delay_cycles(12500000L);
#endif
#ifdef SIGNAL
  signalLow();
#endif
  updateECC(newValue);
#ifdef SIGNAL
  signalHigh();
#endif
  sign_state = uECC_sign(private, hash, sig);
#ifdef SIGNAL
  signalLow();
  __delay_cycles(125000L);
  signalHigh();
#endif
  verify_state = uECC_verify(public, hash, sig);
#ifdef SIGNAL
  signalLow();
#endif
  printECC();
  return sign_state|verify_state;
}
