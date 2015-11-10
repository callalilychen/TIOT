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
  stopWDT();
  ports_init();
  /* Stop WDT and initialize the system-clock of the MCU */
  initClk();
  CLI_Configure();
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
  print("LOS1\n\r");

  unsigned char udp_payloads[MAX_TEST_MESSAGES][BUFSIZE+1];
  unsigned int udp_payloads_size[MAX_TEST_MESSAGES];
  unsigned char send_buf[BUFSIZE+1] = {0};
  char * test_message1 = "gls";
  udp_payloads_size[0] = strlen(test_message1);
  memcpy(udp_payloads[0], test_message1, udp_payloads_size[0]);
  char *test_message2 = "rls";
  udp_payloads_size[1] = strlen(test_message2);
  memcpy(udp_payloads[1], test_message2,  udp_payloads_size[1]);
  // rls with right
  unsigned char test_chars1[] = {
    0x0,0x10,0x2,0x0,0x3,0x4,0x72,0x6c,0x73,0x1b,0x15,0xe8,0xfc,0xae,0x3a,0xf3,0x0,0x56,0xbf,0xc3,0x53,0x26,0x39,0x97,0x70,0x9e,0x74,0x94,0x7c
  };
  udp_payloads_size[2] = sizeof(test_chars1);
  memcpy(udp_payloads[2], test_chars1, udp_payloads_size[2]);
  // rls with wrong MAC
  unsigned char test_chars2[]={
    0x0,0x10,0x2,0x0,0x3,0x4,0x72,0x6c,0x73,0x1b,0x15,0xe8,0xfc,0xae,0x3a,0xf3,0x0,0x56,0xbf,0xc3,0x53,0x26,0x39,0x97,0x70,0x9e,0x74,0x94,0x77
  };
  udp_payloads_size[3] = sizeof(test_chars2);
  memcpy(udp_payloads[3], test_chars2, udp_payloads_size[3]);
  for(int i = 0; i < MAX_TEST_MESSAGES; i++){
    PRINT("Message %d is %u bytes\n", i, udp_payloads_size[i]);
  }
  while(1){
    // Test code  
    for(int i = 0; i < 2; i++){
      TEST_SIGNAL_HIGH;
      __delay_cycles(12500uL);
      TEST_SIGNAL_LOW; 
      DEBUG(">>>>>>>>>>TEST %d<<<<<<<<<<\n", i);
      TEST_SIGNAL_HIGH;
      handleUdpPackage(udp_payloads[i], udp_payloads_size[i], p_addr);
      sendUdpPackage(send_buf, BUFSIZE);
      TEST_SIGNAL_LOW;
    }
    enterLPM();
  }

  return 0;
}

