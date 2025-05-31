#include "value.h"

#include <vector>
#include <cstdlib>
#include <iostream>

ValueArray::ValueArray() {}   // Default

void ValueArray::writeValueArray(Value value) {
  values.push_back(value);
}

void ValueArray::freeValueArray() {
  values.clear();
}

void printValue(Value value) {
  switch (value.type) {
    case VAL_BOOL:
      std::cout << (value.as.boolean ? "true" : "false");
      break;

    case VAL_NIL:
      std::cout << "nil";
      break;

    case VAL_NUMBER:
      std::cout << value.as.number;
      break;
  }
}
