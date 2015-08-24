#ifndef __CONNECTION_H__
#define __CONNECTION_H__
#include "simplelink.h"
#include "sl_common.h"

#ifdef  __cplusplus
extern "C" {
#endif

#define SL_STOP_TIMEOUT        0xFF

/* Use bit 32:
 *      1 in a 'status_variable', the device has completed the ping operation
 *      0 in a 'status_variable', the device has not completed the ping operation
 */
#define STATUS_BIT_PING_DONE  31

#define HOST_NAME       "www.ti.com"

/*
 * Values for below macros shall be modified for setting the 'Ping' properties
 */
#define PING_INTERVAL   1000    /* In msecs */
#define PING_TIMEOUT    3000    /* In msecs */
#define PING_PKT_SIZE   20      /* In bytes */
#define NO_OF_ATTEMPTS  3

/* Application specific status/error codes */
typedef enum{
    LAN_CONNECTION_FAILED = -0x7D0,        /* Choosing this number to avoid overlap with host-driver's error codes */
    INTERNET_CONNECTION_FAILED = LAN_CONNECTION_FAILED - 1,
    DEVICE_NOT_IN_STATION_MODE = INTERNET_CONNECTION_FAILED - 1,

    STATUS_CODE_MAX = -0xBB8
}e_AppStatusCodes;

#define IS_PING_DONE(status_variable)           GET_STATUS_BIT(status_variable, \
                                                               STATUS_BIT_PING_DONE)


extern _u32  g_Status;
extern _u32  g_PingPacketsRecv;
extern _u32  g_GatewayIP;
  
_i32 initializeConnectionVariables(void);
_i32 configureSimpleLinkToDefaultState(void);
_i16 startDevice(const void *, _i8 *, const P_INIT_CALLBACK, _i16);
_i32 establishConnectionWithAP(void);
_i32 checkLanConnection(void);
_i32 checkInternetConnection(void);

#ifdef  __cplusplus
}
#endif /* __cplusplus */

#endif /* __CONNECTION_H__ */

