#include "testcases.h"
#include "crypto/3des.h"

cipher_context_t context;
uint8_t enc_state, dec_state;
uint8_t key[THREEDES_BLOCK_SIZE];
uint8_t pblock_3des[THREEDES_BLOCK_SIZE+1] = "test  !!";
uint8_t cblock_3des[THREEDES_BLOCK_SIZE+1] = "testtest";
uint8_t rpblock_3des[THREEDES_BLOCK_SIZE+1] = "test  !!";


inline int setup3DES(void){
  tripledes_init(&context,THREEDES_BLOCK_SIZE, THREEDES_BLOCK_SIZE, key);
}

inline void update3DES(int i){
  enc_state = 0;
  dec_state = 0;
  pblock_3des[4] = getValidASCII((uint8_t)(i>>8));
  pblock_3des[5] = getValidASCII((uint8_t)i);
}

inline int testEnc3DES(void){
  enc_state = tripledes_encrypt(&context, pblock_3des, cblock_3des);
  return enc_state;
}
inline int testDec3DES(void){
  dec_state = tripledes_decrypt(&context, cblock_3des, rpblock_3des);
  return dec_state;
}
inline void print3DES(void){
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

void test3DES(int i){
  update3DES(i);
#ifdef SIGNAL
  testSignalHigh();
#endif
  testEnc3DES();
#ifdef SIGNAL
  testSignalLow();
#endif
  print3DES();
#ifdef SIGNAL
  testSignalHigh();
#endif
  testDec3DES();
#ifdef SIGNAL
  testSignalLow();
#endif
  print3DES();
}

