#include "object.h"
#include "vm.h"
#include "memory.h"

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

  object->next = vm.objects;
  vm.objects = object;

  return object;
}

ObjString* allocateString(std::vector<char>&& chars, int length) {
  ObjString* string = new ObjString;
  string->length = length;
  string->chars = std::move(chars);
  return string;
}

ObjString* takeString(std::string&& chars, int length) {    // need to manually create vector to move into allocateString
  std::vector<char> buf(chars.data(), chars.data() + length);
  buf.push_back('\0');
  return allocateString(std::move(buf), length);
}

ObjString* copyString(const char* chars, int length) {
  std::vector<char> heapChars(chars, chars + length);
  heapChars.push_back('\0');
  return allocateString(std::move(heapChars), length);
}

void printObject(Value value) {   // Changed from cstring to string
  switch (OBJ_TYPE(value)) {
    case OBJ_STRING: {
      auto* s = AS_STRING(value);
      std::cout.write(s->chars.data(), s->length);
      break;
    }
  }
}


