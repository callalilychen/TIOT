#include <string.h>
#include "testcases.h"
#include "aes.h"
#ifdef CONTIKI
#include "aes-128.h"
#endif
cipher_context_t context;
uint8_t key[AES_KEY_SIZE+1] = "testtesttesttest";
uint8_t pblock_aes[AES_BLOCK_SIZE+1] = "  !testtesttest!";
uint8_t cblock_aes[AES_BLOCK_SIZE+1] = "testtesttesttest";
uint8_t rpblock_aes[AES_BLOCK_SIZE+1] = "  !testtesttest!";

static inline void printAES(void){
  DEBUG("AES DONE\n\r"); 
  DEBUG("Text: %s\n\r", pblock_aes); 
  DEBUG("CText: \n\r"); 
  int i;
  for (i=0; i< AES_BLOCK_SIZE; i++){
    DEBUG("%x", cblock_aes[i]);
  }
  DEBUG("\n\r");
  DEBUG("Text: %s\n\r", rpblock_aes); 
  if(!memcmp(pblock_aes, rpblock_aes, AES_BLOCK_SIZE)){
    DEBUG("AES SUCCESS\n\r");
  }else{
    DEBUG("AES FAILED\n\r");
  }
}

void updateAES(int i){
  pblock_aes[0] = getValidASCII((uint8_t)(i>>8));
  pblock_aes[1] = getValidASCII((uint8_t)i);
}

int setupAES(void){
  aes_init (&context, key, AES_KEY_SIZE);
#ifdef CONTIKI
  aes_128_set_padded_key(key, AES_KEY_SIZE);
#endif
}

void testAES(int i){
#ifdef SIGNAL
  signalHigh();
#endif
#ifdef TESTLPM
  __delay_cycles(12500);
#endif
#ifdef SIGNAL
  signalLow();
#endif
  updateAES(i);
#ifdef SIGNAL
  signalHigh();
#endif
  aes_encrypt(&context, pblock_aes, cblock_aes);
#ifdef SIGNAL
  signalLow();
  signalHigh();
#endif
  aes_decrypt(&context, cblock_aes, rpblock_aes);
#ifdef SIGNAL
  signalLow();
#endif
  printAES();
#ifdef CONTIKI
#ifdef SIGNAL
  signalHigh();
#endif
  memcpy(cblock_aes, pblock_aes, AES_BLOCK_SIZE);
  aes_128_padded_encrypt(cblock_aes,AES_BLOCK_SIZE);
#ifdef SIGNAL
  signalLow();
  signalHigh();
#endif
  memcpy(rpblock_aes, cblock_aes, AES_BLOCK_SIZE);
  aes_128_padded_encrypt(rpblock_aes,AES_BLOCK_SIZE);
#ifdef SIGNAL
  signalLow();
#endif
  printAES();
#endif
}
