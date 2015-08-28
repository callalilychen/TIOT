#ifndef __TREE_H__
#define __TREE_H__

#include <stdio.h>
#include <string.h>
#include "sha256.h"


#ifdef  __cplusplus
extern "C" {
#endif
#define ITEMLEN SHA256_DIGEST_LENGTH
  struct descendant{
    unsigned char * (*function)(unsigned char*, size_t, unsigned char *, size_t, unsigned char *, size_t*);
    unsigned char * param;
    size_t param_len;
    unsigned char * output;
    size_t output_len;
  };
  struct position{
    unsigned int levels;
    struct descendant * descendants;
  };

  void hashfunction(unsigned char *, size_t, unsigned char *);
  void setRoot(unsigned char *, size_t);
  unsigned char * getNode(struct position *, unsigned char *);
#ifdef  __cplusplus
}
#endif /* __cplusplus */

#endif /* __TREE_H__ */

