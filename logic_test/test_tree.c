#include "testcase.h"
#include "interface.h"
#include "tree.h"
#include "hmac.h"
#include "utils.h"
#include <openssl/sha.h>
#include <openssl/hmac.h>
#include <openssl/evp.h>

#define HASH_FUNC sha_construction.func

void g1(tree_node *parent_node, tree_edge *edge, tree_node *node)
{
  unsigned int params_len = 0;
  params_len += (edge->params_size);
  unsigned char input[parent_node->size + params_len];
  memcpy(input, parent_node -> block, parent_node -> size);
  memcpy(input + parent_node -> size, (edge -> params), edge->params_size);
  sha_construction.func(input, parent_node->size + params_len, node->block);
  node->size = sha_construction.size; 
}

unsigned int key_size = 0;

int tree_test(void){
  char* secret = "test";
  tree_node nodes[4] ={0};
  
  HASH_FUNC((unsigned char*)secret, 4, nodes[0].block);
  nodes[0].size = HASH_SIZE;
  printBlock("Root", nodes[0].block, HASH_SIZE);
  
  tree_edge edges[2] = {0};
  const char * params1= "param"; 
  const int params1_size = (unsigned int) strlen(params1);
  edges[0].func = g1;
  edges[0].params = (unsigned char *)params1;
  edges[0].params_size = params1_size;

  fillNodes(nodes, edges, 2, 1);

  printBlock("nodes[1]", nodes[1].block, HASH_SIZE);
  
  unsigned char input[HASH_SIZE + params1_size];
  unsigned char output[HASH_SIZE];

  memcpy(input, nodes[0].block, HASH_SIZE);
  memcpy(input+HASH_SIZE, params1, params1_size);
  SHA256(input, HASH_SIZE + params1_size, output);
  printBlock("test output", output, HASH_SIZE);
  PRINT("len(params) = %u\n", params1_size);

  PRINT("TEST1 hash tree level 2: ");
  if(nodes[1].size == HASH_SIZE && memcmp(nodes[1].block, output, HASH_SIZE) ==0){
    printf("OK\n");
  }else{
    printf("FAILED\n");
  }

  const char * params2= "paran"; 
  unsigned int params2_size =(unsigned int) strlen(params1);
  edges[1].func = g1;
  edges[1].params = (unsigned char*)params2;
  edges[1].params_size = params2_size;

  fillNodes(nodes, edges, 3, 0);

  printBlock("nodes[2]", nodes[2].block, HASH_SIZE);
  
  memcpy(input, nodes[1].block, HASH_SIZE);
  memcpy(input+HASH_SIZE, params2, params2_size);
  SHA256(input, HASH_SIZE + params2_size, output);
  printBlock("test output", output, HASH_SIZE);
  PRINT("len(params) = %u\n", params2_size);

  PRINT("TEST2 hash tree level 3: ");
  if(nodes[2].size == HASH_SIZE && memcmp(nodes[2].block, output, HASH_SIZE) ==0){
    printf("OK\n");
  }else{
    printf("FAILED\n");
  }
  
  return 0;
}

int edge_func_test(void){
  char* secret = "test";
  tree_node nodes[4] ={0};
  
  HASH_FUNC((unsigned char*)secret, 4, nodes[0].block);
  nodes[0].size = HASH_SIZE;
  printBlock("Root", nodes[0].block, HASH_SIZE);
  
  tree_edge edges[2] = {0};

  const char * params1= "para"; 
  unsigned int params1_size = (unsigned int) strlen(params1);
  edges[0].func = edgeFunc;
  edges[0].params = (unsigned char *)params1;
  edges[0].params_size = params1_size;

  fillNodes(nodes, edges, 2, 1);

  printBlock("nodes[1]", nodes[1].block, HASH_SIZE);
  
  unsigned char input[HASH_SIZE];
  unsigned char output[HASH_SIZE];

  for(int i =0; i<HASH_SIZE; i+=params1_size){
    memcpy(input+i, params1, params1_size);
  }
  printBlock("test input", input, HASH_SIZE);
  optimizedXOR(input, nodes[0].block, nodes[0].size, input);
  printBlock("test xored input", input, HASH_SIZE);
  SHA256(input, HASH_SIZE, output);
  printBlock("test output", output, HASH_SIZE);
  PRINT("len(params) = %u\n", params1_size);

  PRINT("TEST1 hash tree level 2: ");
  if(nodes[1].size == HASH_SIZE && memcmp(nodes[1].block, output, HASH_SIZE) ==0){
    printf("OK\n");
  }else{
    printf("FAILED\n");
  }

  const char * params2= "para"; 
  unsigned int params2_size =(unsigned int) strlen(params1);
  edges[1].func = edgeFunc;
  edges[1].params = (unsigned char *)params2;
  edges[1].params_size = params2_size;

  fillNodes(nodes, edges, 3, 0);

  printBlock("nodes[2]", nodes[2].block, HASH_SIZE);

  for(int i =0; i<HASH_SIZE; i+=params2_size){
    memcpy(input+i, params2, params2_size);
  }
  printBlock("test input", input, HASH_SIZE);
  optimizedXOR(input, nodes[1].block, nodes[1].size, input);
  printBlock("test xored input", input, HASH_SIZE);
  SHA256(input, HASH_SIZE, output);
  printBlock("test output", output, HASH_SIZE);
  PRINT("len(params) = %u\n", params2_size);

  PRINT("TEST2 hash tree level 3: ");
  if(nodes[2].size == HASH_SIZE && memcmp(nodes[2].block, output, HASH_SIZE) ==0){
    printf("OK\n");
  }else{
    printf("FAILED\n");
  }

  return 0;

}
