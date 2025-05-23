#include <iostream>

#include "common.h"
#include "chunk.h"
#include "debug.h"


int main(int argc, const char* argv[]) {
  
  Chunk chunk;
  chunk.writeChunk(OP_RETURN);

  disassembleChunk(chunk, "test chunk");

  chunk.freeChunk();

  return 0;

}
