#include "tree.h"
#include "utils.h"

static unsigned char root[ITEMLEN] = {0};
static size_t root_len = 0;

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
  root_len = len;
  if(item == root){
    return;
  }
  if(len > ITEMLEN){
    hashfunction(item, len, root);
  } else {
    memcpy(root, item, len);
  }
}

unsigned char * getNode(struct position * p, unsigned int * size){
  unsigned char * result = root;
  *size = root_len;

  for(int i =0; i<(p->levels); i++){
    descendant * d = p->descendants+i;
    if(d->child_len > 0){
      result = d->child;
      *size = d->child_len;
      continue;
    }
    d->func(d, result, *size);
    result = d->child;
    *size = d->child_len;
  }
  return result;
}

unsigned char * edge(descendant * d,  unsigned char* parent, unsigned int parent_len)
{
  unsigned char p[parent_len];
  size_t p_index = 0;
  while((p_index + d-> param_len) < parent_len){
    memcpy(p+p_index, d->param, d->param_len);
    p_index += d->param_len;
  }
  memcpy(p+p_index,  d->param, parent_len - p_index);
  optimizedXOR(parent, p, parent_len, p);
  hashfunction(p, parent_len, d->child);
  d->child_len = ITEMLEN; 
  return d->child;
}
