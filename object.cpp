#include "object.h"
#include "vm.h"
#include "memory.h"
#include "table.h"

#include <vector>
#include <cstdio>
#include <string>
#include <iostream>
#include <string_view>

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
  std::string_view key{chars.data(), size_t(length)};
  auto it = vm.strings.entries.find(chars);
  if (it != vm.strings.entries.end()) return it->first;            // Map already contains this key
  
  // Not found already so take ownership of bytes
  std::vector<char> buf(chars.data(), chars.data() + length);
  buf.push_back('\0');
  auto stringObj =  allocateString(std::move(buf), length);
  
  vm.strings.entries.emplace(stringObj, OBJ_VAL(stringObj));

  return stringObj;
}

ObjString* copyString(const char* chars, int length) {
  std::string_view key{chars, static_cast<size_t>(length)};
  // Check if the string is unique
  
  auto it = vm.strings.entries.find(key);
  if (it != vm.strings.entries.end()) return it->first;        // key already exits in the map

  std::vector<char> heapChars(chars, chars + length);
  heapChars.push_back('\0');
  auto stringObj =  allocateString(std::move(heapChars), length);
  
  vm.strings.entries.emplace(stringObj, OBJ_VAL(stringObj));
  return stringObj;
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


