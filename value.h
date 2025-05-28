#ifndef value_h
#define value_h

#include "common.h"

#include <vector>

enum ValueType {
  VAL_BOOL,
  VAL_NIL,
  VAL_NUMBER,
};

struct Value {
  ValueType type;
  union {
    bool boolean;
    double number;
  } as;
};

#define IS_BOOL(value) ((value.type) == VAL_BOOL)
#define IS_NIL(value) ((value.type) == VAL_NIL)
#define IS_NUMBER(value) ((value.type) == VAL_NUMBER)

#define AS_BOOL(value) ((value).as.boolean)
#define AS_NUMBER(value) ((value).as.number)

#define BOOL_VAL(value) ((Value){VAL_BOOL, {.boolean = value}})
#define NIL_VAL ((Value){VAL_NIL, {.number = 0}})
#define NUMBER_VAL(value) ((Value){VAL_NUMBER, {.number = value}})

struct ValueArray {                 // Same shtick as chunk, we use vectors instead of arrays
  std::vector<Value> values;

  ValueArray();
  void writeValueArray(Value value); // push_back()
  void freeValueArray();             // clear() 

};

  void printValue(Value value);

#endif
