#include "testcases.h"
int counter = 0;
uint16_t param = 0xf0;
int cases = 0;

void setup(void){
#ifdef SIGNAL
  setupSignal();
  startSignal();
#endif
#ifdef TESTLED
  setupLED();
  cases++;
#endif
#ifdef BUTTON
  setupButton();
  cases++;
#endif
#ifdef TESTLPM
  setupLPM();
#endif
#ifdef SHA256 
  cases++;
#endif
#ifdef ECC 
  setupECC();
  cases++;
#endif
#ifdef AES
  setupAES();
  cases++;
#endif
#ifdef DES
  setup3DES();
  cases++;
#endif
#ifdef DEBUG
  print("Start: %d\n\r", cases);
#endif
}

inline void testSingle(void){
#ifdef LOG 
  print("TestSinle\n\r");
#endif
#ifdef DEBUG
  print("cases: %x\n\r", cases);
  print("counter: %x\n\r", counter);
  print("param: %x\n\r", param);
#endif
  if(param >= 0xff){
    param = 0;
  }else{
  param++;
  }
  if(cases==1){
#ifdef SHA256
    testSHA256(param);
    return;
#endif

#ifdef AES
    testAES(param);
    return;
#endif

#ifdef DES
    test3DES(param);
    return;
#endif

#ifdef ECC
    testECC(param);
    return;
#endif
  }

  if(counter >= cases){
    counter = 0;
  }
  switch(counter){ 
    case 0:
      counter++;
#ifdef SHA256
    testSHA256(param);
  return;
#endif
    case 1:
      counter++;
#ifdef AES
    testAES(param);
  return;
#endif
    case 2:
      counter++;
#ifdef DES
    test3DES(param);
  return;
#endif
    case 3:
      counter++;
#ifdef ECC
    testECC(param);
  return;
#endif
    case 4:
      counter++;
#ifdef BUTTON
  testButton1();
  testButton2();
  return;
#endif 
    case 5:
      counter++;
#ifdef TESTLED
  led1on();
  __delay_cycles(500000);
  led1off();
  __delay_cycles(500000);
  return;
#endif
  }
#ifdef DEBUG
  print("Loop\n\r");
#endif

}

void tests(void){
#ifdef SHA256
    testSHA256(counter);
#endif

#ifdef AES
    testAES(counter);
#endif
#ifdef DES
    test3DES(counter);
#endif

#ifdef ECC
    testECC(counter);
#endif
#ifdef BUTTON
  testButton1();
  testButton2();
#endif 
#ifdef TESTLED
  led1on();
  __delay_cycles(500000);
  led1off();
  __delay_cycles(500000);
#endif
#ifdef DEBUG
  print("Loop\n\r");
#endif
  counter++;

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
