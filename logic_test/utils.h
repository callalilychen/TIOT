#ifndef __UTILS_H__
#define __UTILS_H__

#include <stdint.h>
#include <stdlib.h>

#ifdef  __cplusplus
extern "C" {
#endif

#ifndef WORD_SIZE
#define WORD_SIZE 2
#endif
#if(WORD_SIZE == 1)
#define WORD_TYPE (uint8_t *)
#define WORD_SHIFT_SCALAR 0
#elif(WORD_SIZE == 2)
#define WORD_TYPE (uint16_t *)
#define WORD_SHIFT_SCALAR 1
#elif(WORD_SIZE == 4)
#define WORD_TYPE (uint32_t *)
#define WORD_SHIFT_SCALAR 2
#else
#define WORD_TYPE (uint64_t *)
#define WORD_SHIFT_SCALAR 4
#endif

#define X(a, b, len, res) optimizedXOR(a, b, len, res) 
uint8_t * optimizedXOR(uint8_t *, uint8_t *, size_t, uint8_t *);
#ifdef  __cplusplus
}
#endif /* __cplusplus */

#endif /* __UTILS_H__ */


