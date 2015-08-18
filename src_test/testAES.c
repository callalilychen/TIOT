#include "testcases.h"
#include "crypto/aes.h"
cipher_context_t context;
uint8_t enc_state, dec_state;
uint8_t key[AES_KEY_SIZE];
uint8_t pblock_aes[AES_BLOCK_SIZE+1] = "testtesttest  !!";
uint8_t cblock_aes[AES_BLOCK_SIZE+1] = "testtesttesttest";
uint8_t rpblock_aes[AES_BLOCK_SIZE+1] = "testtesttest  !!";

inline int setupAES(void){
  aes_init (&context, AES_BLOCK_SIZE, AES_KEY_SIZE, key);
}

inline void updateAES(int i){
  enc_state = 0;
  dec_state = 0;
  pblock_aes[12] = getValidASCII((uint8_t)(i>>8));
  pblock_aes[13] = getValidASCII((uint8_t)i);
}

inline int testEncAES(void){
  enc_state = aes_encrypt(&context, pblock_aes, cblock_aes);
  return enc_state;
}
inline int testDecAES(void){
  dec_state = aes_decrypt(&context, cblock_aes, rpblock_aes);
  return dec_state;
}
inline void printAES(void){
#ifdef DEBUG
  if(enc_state){
    print("AES ENC DONE\n\r"); 
    print("Text: %s\n\r", pblock_aes); 
    print("CText: %s\n\r", cblock_aes); 
  }else{
    print("TODO AES ENC!!\n\r");
  }
  if(dec_state){
    print("AES DEC DONE\n\r"); 
    print("CText: %s\n\r", cblock_aes); 
    print("Text: %s\n\r", rpblock_aes); 
  }else{
    print("TODO AES DEC!!\n\r");
  }
#endif
#ifdef INFO
  if(dec_state){
    if(!equal(pblock_aes, rpblock_aes, AES_BLOCK_SIZE)){
      print("AES SUCCESS\n\r");
    }else{
      print("AES FAILED\n\r");
    }
  }
#endif
}

void testAES(int i){
  updateAES(i);
#ifdef SIGNAL
  testSignalHigh();
#endif
  testEncAES();
#ifdef SIGNAL
  testSignalLow();
#endif
  printAES();
#ifdef SIGNAL
  testSignalHigh();
#endif
  testDecAES();
#ifdef SIGNAL
  testSignalLow();
#endif
  printAES();
}

