#include "chunk.h"
#include "memory.h" // May not need since we use vectors instead of arrays

#include <cstdlib>

Chunk::Chunk() {}         // Default constructor doesn't require anything since we're using vectors

void Chunk::writeChunk(uint8_t byte) {
  code.push_back(byte);
}

void Chunk::freeChunk() {
  code.clear();
}
