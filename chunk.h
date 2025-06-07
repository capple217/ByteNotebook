#ifndef chunk_h
#define chunk_h

#include "common.h"
#include "value.h"

#include <vector>

enum OpCode {
  OP_CONSTANT,
  OP_NIL,
  OP_TRUE,
  OP_FALSE,
  OP_POP,
  OP_GET_GLOBAL,
  OP_DEFINE_GLOBAL,
  OP_SET_GLOBAL,
  OP_EQUAL,
  OP_GREATER,
  OP_LESS,
  OP_ADD,
  OP_SUBTRACT,
  OP_MULTIPLY,
  OP_DIVIDE,
  OP_NOT,
  OP_NEGATE,
  OP_PRINT,
  OP_CONSTANT_LONG,                       // Challenge code from Chapter 14 
  OP_RETURN,        // Return from current function
};

struct Chunk {
  // Variables
  std::vector<uint8_t> code;      // Instead of using the raw pointer uint8_t* from the C version, we can be more secure and simpler using a vector
  std::vector<int> lines;         // Line number to track where each line of code is (especially important when debugging code)
  ValueArray constants;
  
  // Functions 
  Chunk();                        // Default Constructor

  void writeChunk(uint8_t byte, int line);    // appends to the back of the vector

  void freeChunk();                 // Empties the vector

  // Functions w ValueArray
  
  int addConstant(Value value);     // Returns the idx to the newest / last value in array

  int writeConstant(Value value, int line);     // Factors in writing constant with more numbers ()
};


#endif
