#include "hmac.h"

#define IPAD 0x36
#define OPAD 0x5c
int hmac(void *, unsigned char* key, size_t key_len, unsigned char* message,  size_t message_len);
