#ifndef __APPLICATION_LAYER_H__
#define __APPLICATION_LAYER_H__

#include <stddef.h>

#ifdef  __cplusplus
extern "C" {
#endif

  unsigned int handleApplicationLayer(unsigned char *, unsigned int, unsigned int);
  unsigned char * generateApplicationLayer(unsigned int *, unsigned int *, unsigned int *);



#ifdef  __cplusplus
}
#endif /* __cplusplus */

#endif /* __APPLICATION_LAYER_H__ */
