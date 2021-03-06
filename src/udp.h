#ifndef __UDP_H__
#define __UDP_H__
#include "simplelink.h"
#include "sl_common.h"
#include "interface.h"

#ifdef  __cplusplus
extern "C" {
#endif
//#define IP_ADDR         0xc0a80164
#define PORT_NUM        5001 
#define BUF_SIZE        20
#define NO_OF_PACKETS   1

  extern SlSockAddr_t diUdpSockAddr;

  int Buf_Put(char);
  _i16 Buf_Read();
  _i16 updateDAddr(SlSockAddr_t * );
  _i16 Buf_Flush(_u8);
  _i32 initializeUDPVariables(void);
  _i16 openBsdUdpSocket(SlSockAddr_t *, _u16);
  _i32 closeBsdUdpSocket(_i16);

  _i32 BsdUdpClient(_u32, _u16);
  _i32 BsdUdpServer(_u16);

#ifdef  __cplusplus
}
#endif /* __cplusplus */

#endif /* __UDP_H__ */

