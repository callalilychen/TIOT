#include "udp.h"
#include "utils.h"

#define NO_OF_PACKETS   1

static _u8 recvBuf[BUF_SIZE+1] = {0};
static _u8 sendBuf[BUF_SIZE+1] = {0};
int bufIdx = 0;
SlSockAddr_t diUdpSockAddr = {0};

int Buf_Put(char c){
  if(bufIdx < BUF_SIZE){
    sendBuf[bufIdx++] = c;
  } else {
    for(int try = 5; try >=0; try --){
      if(Buf_Flush(try==0) == 0){
        return Buf_Put(c);
      }
    }
  }  
}

_i16 updateDAddr(SlSockAddr_t * addr){
  if(addr == 0 || addr == &diUdpSockAddr){
    return -1;
  }
  if(memcmp(addr, &diUdpSockAddr, sizeof(SlSockAddr_t))!=0){
    CLI_Write("Not Same!\r\n");
    Buf_Flush(1);
    memcpy(&diUdpSockAddr, addr, sizeof(SlSockAddr_t));
    return 1;
  }
  return 0;
}

_i16 Buf_Flush(_u8 force){
  if(bufIdx > 0 && (sl_SendTo(udp_socket_fd, sendBuf, bufIdx, 0, &diUdpSockAddr, sizeof(SlSockAddr_t))>=0 || force)){
    bufIdx = 0;
    return 0;
  }
  return -1;
}
struct SlTimeval_t timeout = {
  .tv_sec = 1,
  .tv_usec = 0,
};
_i16 Buf_Read(void)
{
  SlFdSet_t sock_fd;
  SL_FD_ZERO(&sock_fd);
  SL_FD_SET(udp_socket_fd, &sock_fd);
  _i16 len = sl_Select 	( 2, //	_i16  	nfdis,
      &sock_fd, //SlFdSet_t *  	readsds,
      NULL, //SlFdSet_t *  	writesds,
      NULL, //SlFdSet_t *  	exceptsds,
      &timeout //	struct SlTimeval_t *  	timeout 
      );
  _u16 AddrSize = sizeof(SlSockAddr_t);
  for(int i = 0; i < len; i++){
    TEST_SIGNAL_HIGH;
    if(SL_FD_ISSET(udp_socket_fd, &sock_fd)){
      Buf_Flush(1);
    TEST_SIGNAL_LOW;
      _i16 Status = sl_RecvFrom(udp_socket_fd, recvBuf, BUF_SIZE, 0, &diUdpSockAddr, (SlSocklen_t*)&AddrSize);
      if(Status > 0){
        TEST_SIGNAL_HIGH;
        handleUdpPackage(recvBuf, Status, (ADDR_TYPE *)(&diUdpSockAddr));
        sendUdpPackage(sendBuf, BUF_SIZE);
        SL_FD_CLR(udp_socket_fd, &sock_fd);
        TEST_SIGNAL_LOW;
      }
    }
  }
  return len;
}

/* Application specific status/error codes */
typedef enum{
  DEVICE_NOT_IN_STATION_MODE = -0x7D0,        /* Choosing this number to avoid overlap w/ host-driver's error codes */
  BSD_UDP_CLIENT_FAILED = DEVICE_NOT_IN_STATION_MODE - 1,

  STATUS_CODE_MAX = -0xBB8
}e_AppStatusCodes;

_i16 openBsdUdpSocket(SlSockAddr_t * localAddr, _u16 addrSize)
{
  _i16          status = 0;
  _u16          recvSize = 0;

  udp_socket_fd = sl_Socket(SL_AF_INET,SL_SOCK_DGRAM, 0);
  if( udp_socket_fd < udp_socket_fd )
  {
    ASSERT_ON_ERROR(udp_socket_fd);
  }

  status = sl_Bind(udp_socket_fd, localAddr, addrSize);
  if( status < 0 )
  {
    return closeBsdUdpSocket(udp_socket_fd);
  }

  //Enable/disable nonblocking mode
  SlSockNonblocking_t enableOption;
  enableOption.NonblockingEnabled = 1;
  status = sl_SetSockOpt(udp_socket_fd,SL_SOL_SOCKET,SL_SO_NONBLOCKING, (_u8 *)&enableOption,sizeof(enableOption)); 
  ASSERT_ON_ERROR(status);


  return udp_socket_fd;
}

_i32 closeBsdUdpSocket(_i16 sd)
{
  _i16 Status = sl_Close(sd);
  ASSERT_ON_ERROR(Status);

  return SUCCESS;
}

/*!
  \brief This function initializes the application variables

  \param[in]  None

  \return     0 on success, negative error-code on error
  */
      _i32 initializeUDPVariables()
{
  return SUCCESS;
}

