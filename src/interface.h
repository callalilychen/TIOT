#ifndef __INTERFACE_H__
#define __INTERFACE_H__
#include <stdint.h>
#include "printString.h"
#ifdef  __cplusplus
extern "C" {
#endif
#define IPv4 uint32_t
#define IPv6 uint16_t *
typedef IPv4 IPaddr;

#define PRINT print

#ifdef  __cplusplus
}
#endif /* __cplusplus */

#endif /* __INTERFACE_H__ */

