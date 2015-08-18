#ifndef TESTCASES_H
#define TESTCASES_H
#include <stddef.h>
#include "board.h"

#define BUTTONACTION 1000

#define SIGNAL
#define SHA256
//#define AES
//#define DES
//#define TESTLED
#define ECC

void setup(void);
void printError(char * str);
uint8_t getValidASCII(uint8_t i);
int equal(uint8_t *, uint8_t *, int size);

#ifdef SIGNAL
inline void setupSignal(void);
inline void startSignal(void);
inline void testSignalHigh(void);
inline void testSignalLow(void);
#endif

#ifdef TESTLED
#ifndef LED
#define LED
#endif
int testLED1(uint16_t);
int testLED4(uint16_t);
#endif

#ifdef TESTBUTTOM
void setupButton(void);
void testButton1(void);
void testButton2(void);
#endif

#ifdef TESTLPM
void setupLPM(void);
void testLPM(void);
#endif

#ifdef ECC
inline void setupECC(void);
inline void updateECC(int newValue);
inline int testECC(int);
inline void printECC(void);
#endif

#ifdef SHA
inline void setupSHA256(void);
inline void printSHA256(void);
int testSHA256(int);
#endif

#ifdef AES
void testAES(int);
inline int setupAES(void);
inline void updateAES(int);
inline int testEncAES(void);
inline int testDecAES(void);
inline void printAES(void);
#endif

#ifdef DES
void test3DES(int);
inline int setup3DES(void);
inline void update3DES(int);
inline int testEnc3DES(void);
inline int testDec3DES(void);
inline void print3DES(void);
#endif
#endif
