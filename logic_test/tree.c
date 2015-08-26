#include "tree.h"

static unsigned char root[ITEMLEN] = {0};
void setRoot(unsigned char * item)
{
  if(item == root){
    return;
  }
  memcpy(root, item, ITEMLEN);
}
  
unsigned char * getNode(struct position * p, unsigned char * res){
  memcpy(res, root, ITEMLEN);
  for(int i =1; i<(p->levels); i++){
    struct descendant d = p ->descendants[i-1];
    sha256(d.function(res, ITEMLEN, d.param, d.len), ITEMLEN, res);
  }
  return res;
}
