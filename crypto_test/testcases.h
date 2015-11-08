#ifndef TESTCASES_H
#define TESTCASES_H
#include <stddef.h>
#include <string.h>
#include "treeconfig.h"

//#define uECC_SUPPORTS_secp160r1 1
//#define uECC_SUPPORTS_secp192r1 1
//#define uECC_SUPPORTS_secp224r1 0
//#define uECC_SUPPORTS_secp256r1 0
//#define uECC_SUPPORTS_secp256k1 0

#define BUTTONACTION 1000
#define LED

#define SIGNAL
#define CHACHA20
#define SHA256
#define AES
#define DES
#define ECC
#define TESTLPM

#ifdef TESTLPM
#include "lpm.h"
#endif

void setup(void);
void tests(void);
void testSingle(void);
void printError(char * str);
uint8_t getValidASCII(uint8_t i);
int equal(uint8_t *, uint8_t *, int size);

#ifdef SIGNAL
void setupSignal(void);
void startSignal(void);
void testSignalHigh(void);
void testSignalLow(void);
#endif

#ifdef TESTBUTTOM
void setupButton(void);
void testButton1(void);
void testButton2(void);
#endif

#ifdef ECC
void setupECC(void);
int testECC(int);
#endif

#ifdef CHACHA20
int testChaCha20(int);
#endif

#ifdef SHA
int testSHA256(int);
#endif

#ifdef AES
void testAES(int);
int setupAES(void);
#endif

#ifdef DES
void test3DES(int);
int setup3DES(void);
#endif
#endif
uint8_t getValidASCII(uint8_t i);
