#ifndef __LOCAL_INTERFACE_H__
#define __LOCAL_INTERFACE_H__
#include <stdint.h>
#include "printString.h"
#include "sl_common.h"

#ifdef  __cplusplus
extern "C" {
#endif
#define IPv4 uint32_t
#define IPv6 uint16_t *
typedef IPv4 IPaddr;

#define PRINT print
#ifndef SUCCESS
#error SUCCESS must be predefined
#endif

#ifdef  __cplusplus
}
#endif /* __cplusplus */

#endif /* __LOCAL_INTERFACE_H__ */

