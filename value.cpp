#include "value.h"

#include <vector>
#include <cstdlib>

ValueArray::ValueArray() {}   // Default

void ValueArray::writeValueArray(Value value) {
  values.push_back(value);
}

void ValueArray::freeValueArray() {
  values.clear();
}

