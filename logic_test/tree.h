#ifndef __TREE_H__
#define __TREE_H__

#include <stdio.h>
#include <string.h>
#include "sha256.h"


#ifdef  __cplusplus
extern "C" {
#endif
#define ITEMLEN SHA256_DIGEST_LENGTH
  typedef struct descendant{
    unsigned char * (*func)(struct descendant *, unsigned char *, size_t);
    unsigned char * param;
    size_t param_len;
    unsigned char * child;
    size_t child_len;
  } descendant;

  typedef struct position{
    unsigned int levels;
    struct descendant * descendants;
  } position;

  void hashfunction(unsigned char *, size_t, unsigned char *);
  void setRoot(unsigned char *, size_t);
  unsigned char * getNode(struct position *);
  unsigned char * edge(descendant *,  unsigned char*, size_t);
#ifdef  __cplusplus
}
#endif /* __cplusplus */

#endif /* __TREE_H__ */

