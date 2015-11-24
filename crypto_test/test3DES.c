#include "testcases.h"
#include "3des.h"

cipher_context_t context;
uint8_t deskey[24+1]="abcdefghijklmnopqrsnuwyx";
uint8_t pblock_3des[THREEDES_BLOCK_SIZE+1] = "  !test!";
uint8_t cblock_3des[THREEDES_BLOCK_SIZE+1] = "testtest";
uint8_t rpblock_3des[THREEDES_BLOCK_SIZE+1] = "  !test!";


int setup3DES(void){
  tripledes_init(&context,deskey, 24);
}

static inline void print3DES(void){
  DEBUG("3DES DONE\n\r"); 
  DEBUG("Text: %s\n\r", pblock_3des); 
  DEBUG("CText: %s\n\r", cblock_3des); 
  DEBUG("Text: %s\n\r", rpblock_3des); 
  if(!equal(pblock_3des, rpblock_3des, THREEDES_BLOCK_SIZE)){
    DEBUG("DES SUCCESS\n\r");
  }else{
    DEBUG("DES FAILED\n\r");
  }
}
static inline void update3DES(int i){
  pblock_3des[0] = getValidASCII((uint8_t)(i>>8));
  pblock_3des[1] = getValidASCII((uint8_t)i);
}

void test3DES(int i){
#ifdef SIGNAL
  signalHigh();
#endif
#ifdef TESTLPM
  __delay_cycles(12500);
#endif
#ifdef SIGNAL
  signalLow();
#endif
  update3DES(i);
#ifdef SIGNAL
  signalHigh();
#endif
  tripledes_encrypt(&context, pblock_3des, cblock_3des);
#ifdef SIGNAL
  signalLow();
  signalHigh();
#endif
  tripledes_decrypt(&context, cblock_3des, rpblock_3des);
#ifdef SIGNAL
  signalLow();
#endif
  print3DES();
}

