#ifndef __HMAC_H__
#define __HMAC_H__

#include <inttypes.h>
#include <stdlib.h>

#ifdef  __cplusplus
extern "C" {
#endif
int hmac(void *, unsigned char*, size_t, unsigned char*, size_t);
#ifdef  __cplusplus
}
#endif /* __cplusplus */

#endif /* __HMAC_H__ */

