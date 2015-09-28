#ifndef __HANDLER_H__
#define __HANDLER_H__
#include "simplelink.h"
#include "sl_common.h"
#include "connection.h"

#ifdef  __cplusplus
extern "C" {
#endif

void SimpleLinkWlanEventHandler(SlWlanEvent_t *pWlanEvent);
void SimpleLinkNetAppEventHandler(SlNetAppEvent_t *pNetAppEvent);
void SimpleLinkHttpServerCallback(SlHttpServerEvent_t *pHttpEvent,
                                  SlHttpServerResponse_t *pHttpResponse);
void SimpleLinkGeneralEventHandler(SlDeviceEvent_t *pDevEvent);
void SimpleLinkSockEventHandler(SlSockEvent_t *pSock);

#ifdef  __cplusplus
}
#endif /* __cplusplus */

#endif /* __HANDLER_H__ */
