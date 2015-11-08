#include "interface.h"

const hash_function_construction sha_construction = {
  .func = sha256,
  .block_size = 64,
  .size = HASH_SIZE
};
