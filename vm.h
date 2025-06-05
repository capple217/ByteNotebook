#ifndef vm_h
#define vm_h

#include "memory.h"

struct Chunk;  
struct Value;
struct Obj; 

#include <vector>

struct VM {
  Chunk* chunk;                                 // We use pointers here since we're pointing at the data within the VM to the object that has our vector
  uint8_t* ip;
  std::vector<Value> stack;
  Obj* objects;
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
