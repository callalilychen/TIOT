#include "interface.h"
unsigned char *g1(descendant * d,  unsigned char* parent, size_t parent_len)
{
  unsigned char input[parent_len + d->param_len];
  memcpy(input, parent, parent_len);
  memcpy(input + parent_len, d->param, d->param_len);
  HASH_FUNC(input, parent_len + d->param_len, d->child);
  d->child_len = HASH_BLOCK_LENGTH; 
  return d->child;
}



int tree_test(void){
  char* secret = "test";
  unsigned char key[HASH_BLOCK_LENGTH] = {0};
  HASH_FUNC((unsigned char*)secret, 4, key);
  printBlock("key", key, HASH_BLOCK_LENGTH);
  setRoot(key, HASH_BLOCK_LENGTH);
  
  descendant branch[3] = {0};
  position pos = {
    .levels = 1,
    .descendants = branch
  };
  unsigned char * params1= "param"; 
  unsigned char result[3][HASH_BLOCK_LENGTH] = {0};
  branch[0].func = &g1;
  branch[0].param = params1;
  branch[0].param_len = strlen(params1);
  branch[0].child = result[0];
  branch[0].child_len = 0;

  getNode(&pos);

  printBlock("tree[1]", result[0], HASH_BLOCK_LENGTH);
  printBlock("tree[1]", branch[0].child, branch[0].child_len);
  
  unsigned char input[HASH_BLOCK_LENGTH + strlen(params1)];
  memcpy(input, key, HASH_BLOCK_LENGTH);
  memcpy(input+HASH_BLOCK_LENGTH, params1, strlen(params1));
  SHA256(input, HASH_BLOCK_LENGTH + strlen(params1), result[0]);
  printBlock("tree[1]", result[0], HASH_BLOCK_LENGTH);
  PRINT("len(params) = %d\n", strlen(params1));

  unsigned char * params2= "paran"; 
  branch[1].func = &g1;
  branch[1].param = params2;
  branch[1].param_len = strlen(params2);
  branch[1].child = result[1];
  branch[1].child_len = 0;
  pos.levels=2;
  getNode(&pos);

  printBlock("tree[2]", result[1], HASH_BLOCK_LENGTH);
  printBlock("tree[1]", branch[1].child, branch[1].child_len);
  
  memcpy(input, result[0], HASH_BLOCK_LENGTH);
  memcpy(input+HASH_BLOCK_LENGTH, params2, strlen(params2));
  SHA256(input, HASH_BLOCK_LENGTH + strlen(params2), result[1]);
  printBlock("tree[2]", result[1], HASH_BLOCK_LENGTH);
  PRINT("len(params) = %d\n", strlen(params2));
  return 0;
}

int edge_test(void){

  char* secret = "test";
  setRoot(secret, strlen(secret));
  
  descendant branch[3] = {0};
  position pos = {
    .levels = 1,
    .descendants = branch
  };
  unsigned char * params1= "param"; 
  unsigned char result[3][HASH_BLOCK_LENGTH] = {0};
  branch[0].func = &edge;
  branch[0].param = params1;
  branch[0].param_len = strlen(params1);
  branch[0].child = result[0];
  branch[0].child_len = 0;

  getNode(&pos);

  printBlock("tree[1]", result[0], HASH_BLOCK_LENGTH);
  printBlock("tree[1]", branch[0].child, branch[0].child_len);
  
  unsigned char input[HASH_BLOCK_LENGTH] = {0};
  for(int i = 0; i < strlen(secret); i++){
    input[i] = secret[i] ^ params1[i];
  }
  SHA256(input, strlen(secret), result[0]);
  printBlock("tree[1]", result[0], HASH_BLOCK_LENGTH);
  PRINT("len(params) = %d\n", strlen(params1));

  unsigned char * params2= "paran"; 
  branch[1].func = &edge;
  branch[1].param = params2;
  branch[1].param_len = strlen(params2);
  branch[1].child = result[1];
  branch[1].child_len = 0;
  pos.levels=2;
  getNode(&pos);

  printBlock("tree[2]", result[1], HASH_BLOCK_LENGTH);
  printBlock("tree[1]", branch[1].child, branch[1].child_len);
  
  int p_index = 0;
  for(int i = 0; i < HASH_BLOCK_LENGTH; i++){
    input[i] = result[0][i] ^ params2[p_index++];
    if(p_index >= strlen(params2)){
      p_index = 0;
    }
  }
  SHA256(input, HASH_BLOCK_LENGTH, result[1]);
  printBlock("tree[2]", result[1], HASH_BLOCK_LENGTH);
  PRINT("len(params) = %d\n", strlen(params2));

}
