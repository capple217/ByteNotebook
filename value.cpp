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
  std::cout << AS_NUMBER(value);
}
