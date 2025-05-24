#include <iostream>

#include "common.h"
#include "chunk.h"
#include "debug.h"
#include "vm.h"


int main(int argc, const char* argv[]) {

  initVM();           // Might just have an instantiation of the struct instead: VM vm
  
  Chunk chunk;

  int constant = chunk.addConstant(1.2);
  chunk.writeChunk(OP_CONSTANT, 123);
  chunk.writeChunk(constant, 123);

  constant = chunk.addConstant(3.4);
  chunk.writeChunk(OP_CONSTANT, 123);
  chunk.writeChunk(constant, 123);

  chunk.writeChunk(OP_ADD, 123);

  constant = chunk.addConstant(5.6);
  chunk.writeChunk(OP_CONSTANT, 123);
  chunk.writeChunk(constant, 123);

  chunk.writeChunk(OP_DIVIDE, 123);
  chunk.writeChunk(OP_NEGATE, 123);
  chunk.writeChunk(OP_RETURN, 123);

  disassembleChunk(chunk, "test chunk");

  interpret(&chunk);
  freeVM();
  chunk.freeChunk();

  return 0;

}
