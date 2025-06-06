#include "value.h"
#include "object.h"

#include <vector>
#include <string>
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

    case VAL_OBJ: printObject(value); break;
  }
}

bool valuesEqual(Value a, Value b) {
  if (a.type != b.type) return false;
  switch (a.type) {
    case VAL_BOOL:      return AS_BOOL(a) == AS_BOOL(b);
    case VAL_NIL:       return true;
    case VAL_NUMBER:    return AS_NUMBER(a) == AS_NUMBER(b);
    case VAL_OBJ: {
      auto* aStr = AS_STRING(a);
      auto* bStr = AS_STRING(b);
      return (aStr->chars == bStr->chars);
                  }
    default:            return false;                     // unreachable
  }
  
}
