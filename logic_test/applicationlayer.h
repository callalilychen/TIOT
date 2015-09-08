#ifndef __APPLICATION_LAYER_H__
#define __APPLICATION_LAYER_H__

#include <stddef.h>
#include "interface.h"
#include "application_interface.h"

#ifdef  __cplusplus
extern "C" {
#endif

  int handleApplicationLayer(unsigned char *, size_t, unsigned int);
  unsigned char * generateApplicationLayer(unsigned int *, unsigned int *, unsigned int *);
  void clearApplicationLayerSession(unsigned int);

#ifdef  __cplusplus
}
#endif /* __cplusplus */

#endif /* __APPLICATION_LAYER_H__ */
