#include "testcase.h"
#include "interface.h"
#include "tree.h"
#include <openssl/sha.h>
#include <openssl/hmac.h>
#include <openssl/evp.h>

#define HASH_FUNC sha256_construction.func

void * g1(tree_node *parent_node, tree_edge *edge, tree_node *node)
{
  unsigned int params_len = 0;
  for(int i = 0; i< edge->params_counter; i++){
    params_len += (edge->params_size)[i];
  }
  unsigned char input[parent_node->size + params_len];
  memcpy(input, parent_node -> block, parent_node -> size);
  unsigned int curr_len = parent_node -> size;
  for(int i = 0; i< edge->params_counter; i++){
    memcpy(input + curr_len, (edge -> params)[i], (edge->params_size)[i]);
    curr_len += (edge->params_size)[i];
  }
  sha256_construction.func(input, parent_node->size + params_len, node->block);
  node->size = sha256_construction.size; 
}


unsigned int key_size = 0;

int tree_test(void){
  char* secret = "test";
  tree_node nodes[4] ={0};
  
  HASH_FUNC((unsigned char*)secret, 4, nodes[0].block);
  nodes[0].size = HASH_SIZE;
  printBlock("Root", nodes[0], HASH_SIZE);
  
  tree_edge edges[2] = {0};
  unsigned char * params1= "param"; 
  unsigned int params1_size = (unsigned int) strlen(params1);
  edges[0].func = g1;
  edges[0].params = &params1;
  edges[0].params_size = &params1_size;
  edges[0].params_counter = 1;

  fill_nodes(nodes, edges, 1, 1);

  printBlock("nodes[1]", nodes[0].block, HASH_SIZE);
  
  unsigned char input[HASH_SIZE + params1_size];
  memcpy(input, nodes[0].block, HASH_SIZE);
  memcpy(input+HASH_SIZE, params1, params1_size);
  SHA256(input, HASH_SIZE + params1_size,nodes[0].block);
  printBlock("nodes[1]", nodes[1].block, HASH_SIZE);
  PRINT("len(params) = %lu\n", params1_size);

  unsigned char * params2= "paran"; 
  unsigned int params2_size =(unsigned int) strlen(params1);
  edges[1].func = g1;
  edges[1].params = &params2;
  edges[1].params_size = &params2_size;
  edges[1].params_counter = 1;

  fill_nodes(nodes, edges, 2, 0);

  printBlock("nodes[2]", nodes[2].block, HASH_SIZE);
  
  memcpy(input, nodes[1].block, HASH_SIZE);
  memcpy(input+HASH_SIZE, params2, params2_size);
  SHA256(input, HASH_SIZE + params2_size, nodes[2].block);
  printBlock("nodes[2]", nodes[2].block, HASH_SIZE);
  PRINT("len(params) = %lu\n", params2_size);
  return 0;
}

int edge_func_test(void){
  char* secret = "test";
  tree_node nodes[4] ={0};
  
  HASH_FUNC((unsigned char*)secret, 4, nodes[0].block);
  nodes[0].size = HASH_SIZE;
  printBlock("Root", nodes[0], HASH_SIZE);
  
  tree_edge edges[2] = {0};
  unsigned char * params1= "param"; 
  unsigned int params1_size = (unsigned int) strlen(params1);
  edges[0].func = edge_func;
  edges[0].params = &params1;
  edges[0].params_size = &params1_size;
  edges[0].params_counter = 1;

  fill_nodes(nodes, edges, 1, 1);

  printBlock("nodes[1]", nodes[0].block, HASH_SIZE);
  
  unsigned char input[HASH_SIZE + params1_size];
  memcpy(input, nodes[0].block, HASH_SIZE);
  memcpy(input+HASH_SIZE, params1, params1_size);
  SHA256(input, HASH_SIZE + params1_size,nodes[0].block);
  printBlock("nodes[1]", nodes[1].block, HASH_SIZE);
  PRINT("len(params) = %lu\n", params1_size);

  unsigned char * params2= "paran"; 
  unsigned int params2_size =(unsigned int) strlen(params1);
  edges[1].func = edge_func;
  edges[1].params = &params2;
  edges[1].params_size = &params2_size;
  edges[1].params_counter = 1;

  fill_nodes(nodes, edges, 2, 0);

  printBlock("nodes[2]", nodes[2].block, HASH_SIZE);
  
  memcpy(input, nodes[1].block, HASH_SIZE);
  memcpy(input+HASH_SIZE, params2, params2_size);
  SHA256(input, HASH_SIZE + params2_size, nodes[2].block);
  printBlock("nodes[2]", nodes[2].block, HASH_SIZE);
  PRINT("len(params) = %lu\n", params2_size);

  return 0;

}
