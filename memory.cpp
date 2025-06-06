#include "memory.h"
#include "vm.h"
#include "object.h"

#include <cstdlib> 
#include <iostream> 
#include <cstdlib>

void* reallocate(void* pointer, std::size_t oldSize, std::size_t newSize) {
  if (newSize == 0) {
      std::free(pointer);
      return nullptr;
    }
    // When pointer==nullptr, realloc behaves like malloc:
    void* result = std::realloc(pointer, newSize);
    if (result == nullptr) {
        std::cerr << "Out of memory\n";
        std::exit(1);
    }
    return result;
}

void freeObjects() {
  Obj* object = vm.objects;
  
  while (object != nullptr) {
    Obj* next = object->next;
    delete object;
    object = next;
  }

  vm.objects = nullptr;
}
