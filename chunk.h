#ifndef chunk_h
#define chunk_h

#include "common.h"
#include <vector>

enum OpCode {
  OP_RETURN,        // Return from current function
};

struct Chunk {
  std::vector<uint8_t> code;      // Instead of using the raw pointer uint8_t* from the C version, we can be more secure and simpler using a vector
  
  Chunk();                        // Default Constructor

  void writeChunk(uint8_t byte);    // appends to the back of the vector

  void freeChunk();                 // Empties the vector
};


#endif
