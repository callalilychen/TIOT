#include <stdint.h>


#include "simplelink.h"
#include "sl_common.h"
#include "button.h"
#include "wdt.h"
#include "connection.h"
#include "udp.h"
#include "handler.h"
#include "board.h"

#define APPLICATION_VERSION "1.2.0"
#define AS_PORT 9001
#define AS_IP SL_IPV4_VAL(127,0,0,1)

/*
 * STATIC FUNCTION DEFINITIONS -- Start
 */
static void displayBanner();
/*
 * STATIC FUNCTION DEFINITIONS -- End
 */

static void printIP(char* name, uint32_t ip){
  print(name);
  print(":0x%w\n", ip);
  for(int i = 24; i >= 0; i-=8){
    uint8_t t = (uint8_t)(ip >> i);
    print(".%d", t);
  }
  print("\n");
}

/*
 * Application's entry point
 */
int main(int argc, char** argv)
{
  /* Stop WDT and initialize the system-clock of the MCU */
  stopWDT();
  initClk();
  CLI_Configure();
  print("LOS1\n\r");
  initLEDs();

  displayBanner();
  /*
   * Following function configures the device to default state by cleaning
   * the persistent settings stored in NVMEM (viz. connection profiles &
   * policies, power policy etc)
   *
   * Applications may choose to skip this step if the developer is sure
   * that the device is in its default state at start of application
   *
   * Note that all profiles and persistent settings that were done on the
   * device will be lost
   */
  /* Configure command line interface */
  _i32 retVal = -1;

  retVal = initializeConnectionVariables();
  ASSERT_ON_ERROR(retVal);

  while((retVal = configureSimpleLinkToDefaultState()) < 0)
  {
    if (DEVICE_NOT_IN_STATION_MODE == retVal)
      CLI_Write((_u8 *)" Failed to configure the device in its default state \n\r");

    LOOP_FOREVER();
  }

  CLI_Write((_u8 *)" Device is configured in default state \n\r");

  /*
   * Assumption is that the device is configured in station mode already
   * and it is in its default state
   */
  while((retVal = startDevice(0,0,0,ROLE_STA)) < 0)
  {
    CLI_Write((_u8 *)" Failed to start the device \n\r");
  }

  CLI_Write((_u8 *)" Device started as STATION \n\r");

  /* Connecting to WLAN AP */
  while((retVal = establishConnectionWithAP())<0)
  {
    CLI_Write((_u8 *)" Failed to establish connection w/ an AP \n\r");
  }

  CLI_Write((_u8 *)" Connection established w/ AP and IP is acquired \n\r");
  _u8 macAddressVal[SL_MAC_ADDR_LEN];
  _u8 macAddressLen = SL_MAC_ADDR_LEN;
  sl_NetCfgGet(SL_MAC_ADDRESS_GET,NULL,&macAddressLen,(_u8 *)macAddressVal);
  print("\nMAC");
  for(int i=0; i<macAddressLen; i++){
    print(":%x", macAddressVal[i]);
  }
  _u8 len = sizeof(SlNetCfgIpV4Args_t);
  print("\nlen=%d\n", len);
  _u8 dhcpIsOn = 0;
  SlNetCfgIpV4Args_t ipV4 = {0};
  sl_NetCfgGet(SL_IPV4_STA_P2P_CL_GET_INFO,&dhcpIsOn,&len,(_u8 *)&ipV4);
  print("\nIP DHCPIsON=%u\n", dhcpIsOn);
  print("len=%d\n", len);
  printIP("ipv4", ipV4.ipV4);
  printIP("ipv4DnsServer", ipV4.ipV4DnsServer);
  printIP("ipv4Gateway", ipV4.ipV4Gateway);
  printIP("ipv4Mask", ipV4.ipV4Mask);


  CLI_Write((_u8 *)" Pinging...! \n\r");

  while((retVal = checkLanConnection()) < 0)
  {
    CLI_Write((_u8 *)" Device couldn't connect to LAN \n\r");
  }

  CLI_Write((_u8 *)" Device successfully connected to the LAN\r\n");

  retVal = initializeUDPVariables();
  CLI_Write(" Waiting for data from UDP client \n\r");

  _i16          Status = 0;
  SlSockAddrIn_t  LocalAddr = {0};
  LocalAddr.sin_family = SL_AF_INET;
  LocalAddr.sin_port = sl_Htons(PORT_NUM);
  LocalAddr.sin_addr.s_addr = 0;

  while(openBsdUdpSocket((SlSockAddr_t *)&LocalAddr, sizeof(SlSockAddrIn_t))<0)
    CLI_Write(" Failed to read data from the UDP socket \n\r");

  resetAllExpectedStates();
  resetApplicationSession();

  // TODO config root 
  const char root[5] = "test";
  setRoot((unsigned char *)root, 4);

  /* Init predef addr of AS */
  updateAddrDescriptor(PREDEF_AS_ADDR, AS_PORT, AS_IP);

  initWDT(WDT_ADLY_16);
  setupButton(15, 50);
  _u16 AddrSize = sizeof(SlSockAddrIn_t);
  char * buf1 = "B1 was pressed\n";
  char * buf2 = "B2 was pressed\n";
  int (*handleFunction)(char);
  while(1){
    Buf_Read();

    if(!updateState()){
      continue;
    }
    int b1type = b1pressed();
    int b2type = b2pressed();
    if( b1type){
      print("B1 was pressed %d time\n", b1type);
      handleFunction = (int (*)(char))getHandler();
      setHandler(&Buf_Put);
      print("B1 was pressed %d time\n", b1type);
      setHandler(&CLI_Put);
    }
    if( b2type){
      print("B2 was pressed %d time\n", b2type);
      handleFunction =  (int (*)(char))getHandler();
      setHandler(&Buf_Put);
      print("B2 was pressed %d time\n", b2type);
      setHandler(handleFunction);
    }
  }


  /* Stop the CC3100 device */
  retVal = sl_Stop(SL_STOP_TIMEOUT); 

  return 0;
}

/*!
  \brief This function displays the application's banner

  \param      None

  \return     None
  */
static void displayBanner()
{
  CLI_Write((_u8 *)"\n\r\n\r");
  CLI_Write((_u8 *)" Getting started with station application - Version ");
  CLI_Write((_u8 *) APPLICATION_VERSION);
  CLI_Write((_u8 *)"\n\r*******************************************************************************\n\r");
}
