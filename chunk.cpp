#include "chunk.h"
#include "memory.h" // May not need since we use vectors instead of arrays

#include <cstdlib>

Chunk::Chunk() {}         // Default constructor doesn't require anything since we're using vectors

void Chunk::writeChunk(uint8_t byte, int line) {
  code.push_back(byte);
  lines.push_back(line);
} // Current approach simply push_back instead of indexing the end; hopefully ok

void Chunk::freeChunk() {
  code.clear();
}

int Chunk::addConstant(Value value) {
  constants.writeValueArray(value);
  return constants.values.size() - 1;
}

int Chunk::writeConstant(Value value, int line) {
  int idx = addConstant(value);
  
  if (idx <= UINT8_MAX) {
    writeChunk(OP_CONSTANT, line);
    writeChunk(static_cast<uint8_t>(idx), line);
  }
  else {
    writeChunk(OP_CONSTANT_LONG, line);
    writeChunk((idx >> 16) & 0xFF, line);
    writeChunk((idx >> 8) & 0xFF, line);
    writeChunk(idx & 0xFF, line);
  }

  return idx;
}
