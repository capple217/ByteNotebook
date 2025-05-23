#ifndef debug_h
#define debug_h

#include "chunk.h"

void disassembleChunk(Chunk& chunk, const char* name);
int disassembleChunk(Chunk& chunk, int offset);

#endif
