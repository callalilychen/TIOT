#include "udp.h"
union
{
    _u8 BsdBuf[BUF_SIZE];
    _u32 demobuf[BUF_SIZE/4];
} uBuf;
_u8 recvBuf[BUF_SIZE+1] = {0};

/* Application specific status/error codes */
typedef enum{
    DEVICE_NOT_IN_STATION_MODE = -0x7D0,        /* Choosing this number to avoid overlap w/ host-driver's error codes */
    BSD_UDP_CLIENT_FAILED = DEVICE_NOT_IN_STATION_MODE - 1,

    STATUS_CODE_MAX = -0xBB8
}e_AppStatusCodes;

_i16 openBsdUdpSocket(SlSockAddr_t * localAddr, _u16 addrSize)
{
    _i16          SockID = 0;
    _i16          Status = 0;
    _u16          recvSize = 0;

    SockID = sl_Socket(SL_AF_INET,SL_SOCK_DGRAM, 0);
    if( SockID < 0 )
    {
        ASSERT_ON_ERROR(SockID);
    }

    Status = sl_Bind(SockID, localAddr, addrSize);
    if( Status < 0 )
    {
        return closeBsdUdpSocket(SockID);
    }

    return SockID;
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
    pal_Memset(uBuf.BsdBuf, 0, sizeof(uBuf));

    return SUCCESS;
}


/*!
    \brief Opening a UDP client side socket and sending data

    This function opens a UDP socket and tries to send data to a UDP server
    IP_ADDR waiting on port PORT_NUM.
    Then the function will send 1000 UDP packets to the server.

    \param[in]      port number on which the server will be listening on

    \return         0 on success, -1 on Error.

    \note

    \warning
*/
_i32 testBsdUdpClient(IPaddr server_ip, _u16 Port)
{
    SlSockAddrIn_t  Addr;
    _u16            idx = 0;
    _u16            AddrSize = 0;
    _i16            SockID = 0;
    _i16            Status = 0;
    _u16            LoopCount = 0;

    for (idx=0 ; idx<BUF_SIZE ; idx++)
    {
        uBuf.BsdBuf[idx] = (_u8)(idx % 10);
    }

    Addr.sin_family = SL_AF_INET;
    Addr.sin_port = sl_Htons((_u16)Port);
    Addr.sin_addr.s_addr = sl_Htonl(server_ip);

    AddrSize = sizeof(SlSockAddrIn_t);

    print("SockID is %d", SockID);
    SockID = sl_Socket(SL_AF_INET,SL_SOCK_DGRAM, 0);

    print("SockID is %d", SockID);
    if( SockID < 0 )
    {
        ASSERT_ON_ERROR(SockID);
    }

    while (LoopCount < NO_OF_PACKETS)
    {
        Status = sl_SendTo(SockID, uBuf.BsdBuf, BUF_SIZE, 0,
                               (SlSockAddr_t *)&Addr, AddrSize);
        if( Status <= 0 )
        {
            Status = sl_Close(SockID);
            ASSERT_ON_ERROR(BSD_UDP_CLIENT_FAILED);
        }

        LoopCount++;
    }

    Status = sl_Close(SockID);
    ASSERT_ON_ERROR(Status);

    return SUCCESS;
}

/*!
    \brief Opening a UDP server side socket and receiving data

    This function opens a UDP socket in Listen mode and waits for incoming
    UDP packets from the connected client.

    \param[in]      port number on which the server will be listening on

    \return         0 on success, Negative value on Error.

    \note

    \warning
*/
 _i32 testBsdUdpServer(_u16 Port)
{
    SlSockAddrIn_t  Addr;
    SlSockAddrIn_t  LocalAddr;
    _u16          idx = 0;
    _u16          AddrSize = 0;
    _i16          SockID = 0;
    _i16          Status = 0;
    _u16          LoopCount = 0;
    _u16          recvSize = 0;

    for (idx=0 ; idx<BUF_SIZE ; idx++)
    {
        uBuf.BsdBuf[idx] = (_u8)(idx % 10);
    }

    LocalAddr.sin_family = SL_AF_INET;
    LocalAddr.sin_port = sl_Htons((_u16)Port);
    LocalAddr.sin_addr.s_addr = 0;

    AddrSize = sizeof(SlSockAddrIn_t);
    print("AddrSize = %d\n", AddrSize); 

    print("SockID is %d\n", SockID);
    SockID = sl_Socket(SL_AF_INET,SL_SOCK_DGRAM, 0);
    print("SockID is %d\n", SockID);
    if( SockID < 0 )
    {
        ASSERT_ON_ERROR(SockID);
    }

    Status = sl_Bind(SockID, (SlSockAddr_t *)&LocalAddr, AddrSize);
    print("%d = sl_Bind(%d, {%d,%d,%d}, %d)\n", Status, SockID, 
        LocalAddr.sin_addr.s_addr, LocalAddr.sin_family, LocalAddr.sin_port,
        AddrSize);
    if( Status < 0 )
    {
        Status = sl_Close(SockID);
        ASSERT_ON_ERROR(Status);
    }

    while (LoopCount < NO_OF_PACKETS)
    {
      print("%d < %d\n", LoopCount, NO_OF_PACKETS);
        recvSize = 0;

        do
        {
            Status = sl_RecvFrom(SockID, recvBuf+recvSize, BUF_SIZE-recvSize, 0,
                                (SlSockAddr_t *)&Addr, (SlSocklen_t*)&AddrSize );
            print("%d = sl_RecvFrom(%d, %d)\n", Status, SockID, BUF_SIZE-recvSize);
            print("recvBuf = %s\n", recvBuf);
            if(Status < 0)
            {
                sl_Close(SockID);
                ASSERT_ON_ERROR(Status);
            }

            recvSize += Status;

        }while(recvSize < BUF_SIZE);

        LoopCount++;
    }

    Status = sl_Close(SockID);
    ASSERT_ON_ERROR(Status);

    return SUCCESS;
}

