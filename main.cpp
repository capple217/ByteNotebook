#include <iostream>

#include "common.h"
#include "chunk.h"


int main(int argc, const char* argv[]) {
  
  Chunk chunk;
  chunk.writeChunk(OP_RETURN);
  chunk.freeChunk();

  return 0;

}
