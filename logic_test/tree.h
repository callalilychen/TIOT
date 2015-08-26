#ifndef __TREE_H__
#define __TREE_H__

#include <stdio.h>
#include <string.h>
#include "interface.h"

#ifdef  __cplusplus
extern "C" {
#endif
#define ITEMLEN 32
  struct descendant{
    unsigned char * (*function)(unsigned char*, int, unsigned char *, int);
    unsigned char * param;
    int len;
  };
  struct position{
    unsigned int levels;
    struct descendant * descendants;
  };
  void setRoot(unsigned char *);
  unsigned char * getNode(struct position *, unsigned char *);
#ifdef  __cplusplus
}
#endif /* __cplusplus */

#endif /* __TREE_H__ */

