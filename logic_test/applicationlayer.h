#ifndef __APPLICATION_LAYER_H__
#define __APPLICATION_LAYER_H__

#include "interface.h"
#include "treestate.h" 

#ifdef  __cplusplus
extern "C" {
#endif


int handleApplicationLayer(unsigned char *, size_t, unsigned char **, size_t *, unsigned char* res, size_t *);


#ifdef  __cplusplus
}
#endif /* __cplusplus */

#endif /* __APPLICATION_LAYER_H__ */
