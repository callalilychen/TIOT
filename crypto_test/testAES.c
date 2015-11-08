#include <string.h>
#include "testcases.h"
#include "aes.h"
cipher_context_t context;
uint8_t enc_state, dec_state;
uint8_t key[AES_KEY_SIZE];
uint8_t pblock_aes[AES_BLOCK_SIZE+1] = "testtesttest  !!";
uint8_t cblock_aes[AES_BLOCK_SIZE+1] = "testtesttesttest";
uint8_t rpblock_aes[AES_BLOCK_SIZE+1] = "testtesttest  !!";

static inline void printAES(void){
  if(enc_state){
    DEBUG("AES ENC DONE\n\r"); 
    DEBUG("Text: %s\n\r", pblock_aes); 
    DEBUG("CText: %s\n\r", cblock_aes); 
  }else{
    DEBUG("TODO AES ENC!!\n\r");
  }
  if(dec_state){
    DEBUG("AES DEC DONE\n\r"); 
    DEBUG("CText: %s\n\r", cblock_aes); 
    DEBUG("Text: %s\n\r", rpblock_aes); 
  }else{
    DEBUG("TODO AES DEC!!\n\r");
  }
  if(dec_state){
    if(!memcmp(pblock_aes, rpblock_aes, AES_BLOCK_SIZE)){
      DEBUG("AES SUCCESS\n\r");
    }else{
      DEBUG("AES FAILED\n\r");
    }
  }
}

int setupAES(void){
  aes_init (&context, key, AES_BLOCK_SIZE);
}

int testEncAES(void){
#ifdef SIGNAL
  signalHigh();
#endif
  enc_state = aes_encrypt(&context, pblock_aes, cblock_aes);
#ifdef SIGNAL
  signalLow();
#ifdef TESTLPM
  __delay_cycles(32);
#else
  __delay_cycles(1600);
  __delay_cycles(1600);
#endif
#endif
  printAES();
  return enc_state;
}
int testDecAES(void){
#ifdef SIGNAL
  signalHigh();
#endif
  dec_state = aes_decrypt(&context, cblock_aes, rpblock_aes);
#ifdef SIGNAL
  signalLow();
#ifndef TESTLPM
  __delay_cycles(1600);
  __delay_cycles(1600);
#endif
#endif
  printAES();
  return dec_state;
}

void updateAES(int i){
  enc_state = 0;
  dec_state = 0;
  pblock_aes[12] = getValidASCII((uint8_t)(i>>8));
  pblock_aes[13] = getValidASCII((uint8_t)i);
}

void testAES(int i){
  updateAES(i);
  testEncAES();
  testDecAES();
}
