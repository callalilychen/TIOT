#ifndef __HMAC_H__
#define __HMAC_H__

#include <string.h>
#include <strings.h>

#ifdef  __cplusplus
extern "C" {
#endif
void hmac(void (*)(unsigned char *, size_t, unsigned char *), size_t, unsigned char*, size_t, unsigned char*, size_t, unsigned char*);
#ifdef  __cplusplus
}
#endif /* __cplusplus */

#endif /* __HMAC_H__ */

