#ifndef vm_h
#define vm_h

#include "memory.h"
#include "value.h"
#include "chunk.h"
#include "table.h"

#include <vector>
#include <string>
#include <unordered_map>

struct VM {
  Chunk* chunk;                                 // We use pointers here since we're pointing at the data within the VM to the object that has our vector
  uint8_t* ip;
  std::vector<Value> stack;
  Obj* objects;
  Table strings;
};

  void initVM();

  void freeVM();

enum InterpretResult {
  INTERPRET_OK,
  INTERPRET_COMPILE_ERROR,
  INTERPRET_RUNTIME_ERROR,
};

  extern VM vm;

  InterpretResult interpret(const std::string source);

  void push(Value value);

  Value pop();


#endif
