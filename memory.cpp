#include "memory.h"

void freeObjects() {
  Obj* object = vm.objects;
  
  while (object != nullptr) {
    Obj* next = object->next;
    delete object;
    object = next;
  }

  vm.objects = nullptr;
}
