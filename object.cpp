#include "object.h"
#include "value.h"
#include "vm.h"

#include <vector>
#include <cstdio>
#include <string>
#include <iostream>

static Obj* allocateObject(size_t size, ObjType type) {
  // Allocate raw memory via our custom allocator
  void* mem = reallocate(nullptr, 0, size);
  //cast it to Obj*
  Obj* object = reinterpret_cast<Obj*>(mem);
  object->type = type;
  return object;
}

ObjString* allocateString(std::vector<char>&& chars, int length) {
  ObjString* string = new ObjString;
  string->length = length;
  string->chars = std::move(chars);
}

ObjString* copyString(const char* chars, int length) {
  std::vector<char> heapChars(chars, chars + length);
  heapChars.push_back('\0');
  return allocateString(std::move(heapChars), length);
}

void printObject(Value value) {
  switch (OBJ_TYPE(value)) {
    case OBJ_STRING: {
      cosnt char* s = AS_CSTRING(value);
      std::cout << s;
      break;
    }
  }
}

