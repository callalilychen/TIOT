#include "testcases.h"
#include "3des.h"

cipher_context_t context;
uint8_t enc_state, dec_state;
uint8_t key[THREEDES_BLOCK_SIZE];
uint8_t pblock_3des[THREEDES_BLOCK_SIZE+1] = "test  !!";
uint8_t cblock_3des[THREEDES_BLOCK_SIZE+1] = "testtest";
uint8_t rpblock_3des[THREEDES_BLOCK_SIZE+1] = "test  !!";


int setup3DES(void){
  tripledes_init(&context,key, THREEDES_BLOCK_SIZE);
}

void print3DES(void){
#ifdef DEBUG
  if(enc_state){
    print("3DES ENC DONE\n\r"); 
    print("Text: %s\n\r", pblock_3des); 
    print("CText: %s\n\r", cblock_3des); 
  }else{
    print("TODO 3DES ENC!!\n\r");
  }
  if(dec_state){
    print("3DES DEC DONE\n\r"); 
    print("CText: %s\n\r", cblock_3des); 
    print("Text: %s\n\r", rpblock_3des); 
  }else{
    print("TODO 3DES DEC!!\n\r");
  }
#endif
#ifdef INFO
  if(dec_state){
    if(!equal(pblock_3des, rpblock_3des, THREEDES_BLOCK_SIZE)){
      print("DES SUCCESS\n\r");
    }else{
      print("DES FAILED\n\r");
    }
  }
#endif
}
void update3DES(int i){
  enc_state = 0;
  dec_state = 0;
  pblock_3des[4] = getValidASCII((uint8_t)(i>>8));
  pblock_3des[5] = getValidASCII((uint8_t)i);
}

int testEnc3DES(void){
#ifdef SIGNAL
  signalHigh();
#endif
  enc_state = tripledes_encrypt(&context, pblock_3des, cblock_3des);
#ifdef SIGNAL
  signalLow();
#ifdef TESTLPM
  __delay_cycles(32);
#else
  __delay_cycles(1600);
  __delay_cycles(1600);
  __delay_cycles(1600);
#endif
#endif
  print3DES();
  return enc_state;
}
int testDec3DES(void){
#ifdef SIGNAL
  signalHigh();
#endif
  dec_state = tripledes_decrypt(&context, cblock_3des, rpblock_3des);
#ifdef SIGNAL
  signalLow();
#ifndef TESTLPM
  __delay_cycles(1600);
  __delay_cycles(1600);
  __delay_cycles(1600);
#endif
#endif
  print3DES();
  return dec_state;
}

void test3DES(int i){
  update3DES(i);
  testEnc3DES();
  testDec3DES();
}

