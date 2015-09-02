#ifndef __HMAC_H__
#define __HMAC_H__

#include <string.h>
#include <strings.h>
#include "interface.h"

#ifdef  __cplusplus
extern "C" {
#endif
#ifndef WORD_SIZE
#define WORD_SIZE 2
#define WORD_TYPE (uint16_t *)
#endif

unsigned int hmac(void (*)(unsigned char *, size_t, unsigned char *), unsigned int, unsigned char*, unsigned int, unsigned char*, unsigned int, unsigned char*);
#ifdef  __cplusplus
}
#endif /* __cplusplus */

#endif /* __HMAC_H__ */

