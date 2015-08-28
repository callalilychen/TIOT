#include "tree.h"

static unsigned char root[ITEMLEN] = {0};

void hashfunction(unsigned char *d, size_t n, unsigned char * md)
{
  sha256_context_t c;
  static unsigned char m[ITEMLEN];

  if (md == NULL) {
      md = m;
  }

  sha256_init(&c);
  sha256_update(&c, d, n);
  sha256_final(md, &c);
}
void setRoot(unsigned char * item, size_t len)
{
  if(item == root){
    return;
  }
  if(len > ITEMLEN){
    hashfunction(item, len, root);
  } else {
    memcpy(root, item, len);
  }
}
  
unsigned char * getNode(struct position * p, unsigned char * res){
  memcpy(res, root, ITEMLEN);
  unsigned char temp[ITEMLEN*2];
  size_t len;
  for(int i =1; i<(p->levels); i++){
    struct descendant d = p ->descendants[i-1];
    hashfunction(d.function(res, ITEMLEN, d.param, d.param_len, temp, &len), len, res);
  }
  return res;
}
