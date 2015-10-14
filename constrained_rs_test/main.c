#include <stdint.h>
#include "board_ext.h"
#include "treeconfig.h"
#include "lpm.h"
#include "packagehandler.h"

#define AS_PORT 9001
#define AS_IP SL_IPV4_VAL(127,0,0,1)
#define MAX_TEST_MESSAGES 4
#define MESSAGE_INDEX 0 
/*
 * Application's entry point
 */
int main(int argc, char** argv)
{
  ports_init();
  /* Stop WDT and initialize the system-clock of the MCU */
  stopWDT();
  initClk();
  CLI_Configure();
  print("LOS1\n\r");
  initLEDs();

  resetAllExpectedStates();
  resetApplicationSession();

  /* Init predef addr of AS */
  updateAddrDescriptor(PREDEF_AS_ADDR, AS_PORT, AS_IP);

  ADDR_TYPE * p_addr = getAddr(PREDEF_AS_ADDR);
  // TODO config root 
  const char root[5] = "test";
  setRoot((unsigned char *)root, 4);
  setupLPM(LPM3_bits + GIE);// Enter LPM3

  unsigned char udp_payloads[MAX_TEST_MESSAGES][BUFSIZE+1];
  unsigned int udp_payloads_size[MAX_TEST_MESSAGES];
  unsigned char send_buf[BUFSIZE+1] = {0};
  char * test_message1 = "gls";
  memcpy(udp_payloads[0], test_message1, strlen(test_message1));
  char *test_message2 = "gls";
  memcpy(udp_payloads[1], test_message2, strlen(test_message2));
  unsigned char test_chars1[] = {0x0,0x0,0x3,0x0,0x2,0x72,0x6c,0x73,0x53,0xa7,0x7d,0x69,0x9b,0x2e,0xcc,0x4,0x7c,0x1e,0x30,0x74,0x17,0xa8,0x9,0xde,0xbe,0xd2,0x84,0x9a,0x0};
  memcpy(udp_payloads[2], test_chars1, sizeof(test_chars1));
  unsigned char test_chars2[]={0x0,0x0,0x4,0x3,0x3,0x72,0x6c,0x73,0xa8,0xb3,0x22,0x81,0x61,0x6d,0x41,0x19,0x72,0xaa,0x0,0x29,0xa0,0x51,0xf5,0x85,0x36,0x66,0x8f,0xe4, 0x0};
  memcpy(udp_payloads[3], test_chars2, sizeof(test_chars2));
  for(int i = 0; i < MAX_TEST_MESSAGES; i++){
    PRINT("Message %d is %u bytes\n", i, udp_payloads_size[i]);
  }
  while(1){
  // Test code  
    TEST_SIGNAL_HIGH;
    handleUdpPackage(udp_payloads[MESSAGE_INDEX], udp_payloads_size[MESSAGE_INDEX], p_addr);
    sendUdpPackage(send_buf, BUFSIZE);
    TEST_SIGNAL_LOW;
    enterLPM(); 
  }

  return 0;
}

