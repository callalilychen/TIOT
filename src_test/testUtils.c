#include "testcases.h"
void setup(void){
#ifdef TESTLED
  setupLED();
#endif
#ifdef BUTTON
  setupButton();
#endif
#ifdef LPM
  setupLPM();
#endif
#ifdef ECC 
  setupECC();
#endif
#ifdef AES
  setupAES();
#endif
#ifdef DES
  setup3DES();
#endif
#ifdef SIGNAL
  setupSignal();
  startSignal();
#endif
}

uint8_t getValidASCII(uint8_t i){
  if(i<33){
    return i+33;
  }
  if(i>126){
    i=126;
  }
}

void printError(char * str)
{
  print("%s ERROR\n\r", str);

}
int equal(uint8_t * a1, uint8_t * a2, int size){
  int i = 0;
  for (; i< size; i++){
    if(a1[i] != a2[i]){
      break;
    }
  }
  return size-i;
}
