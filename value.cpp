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

bool valuesEqual(Value a, Value b) {
  if (a.type != b.type) return false;
  switch (a.type) {
    case VAL_BOOL:      return AS_BOOL(a) == AS_BOOL(b);
    case VAL_NIL:       return true;
    case VAL_NUMBER:    return AS_NUMBER(a) == AS_NUMBER(b);
    default:            return false;                     // unreachable
  }
  
}
