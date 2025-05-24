#ifndef value_h
#define value_h

#include "common.h"

#include <vector>

using Value = double;

struct ValueArray {                 // Same shtick as chunk, we use vectors instead of arrays
  std::vector<Value> values;

  ValueArray();
  void writeValueArray(Value value); // push_back()
  void freeValueArray();             // clear() 

};

  void printValue(Value value);

#endif
