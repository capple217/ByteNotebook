#ifndef value_h
#define value_h

#include "common.h"
#include "memory.h"

#include <vector>

struct Obj;
struct ObjString;

enum ValueType {
  VAL_BOOL,
  VAL_NIL,
  VAL_NUMBER,
  VAL_OBJ,
};

struct Value {
  ValueType type;
  union {
    bool boolean;
    double number;
    Obj* obj;
  } as;
};

#define IS_BOOL(value) ((value.type) == VAL_BOOL)
#define IS_NIL(value) ((value.type) == VAL_NIL)
#define IS_NUMBER(value) ((value.type) == VAL_NUMBER)
#define IS_OBJ(value) ((value.type) == VAL_OBJ)

#define AS_OBJ(value) ((value).as.obj)                      // Extracts the Obj pointer from the value
#define AS_BOOL(value) ((value).as.boolean)
#define AS_NUMBER(value) ((value).as.number)

#define BOOL_VAL(value) ((Value){VAL_BOOL, {.boolean = value}})
#define NIL_VAL ((Value){VAL_NIL, {.number = 0}})
#define NUMBER_VAL(value) ((Value){VAL_NUMBER, {.number = value}})
#define OBJ_VAL(object) ((Value){VAL_OBJ, {.obj = (Obj*)object}})       // Takes a bare Obj pointer and wraps it in a full Value

struct ValueArray {                 // Same shtick as chunk, we use vectors instead of arrays
  std::vector<Value> values;

  ValueArray();
  void writeValueArray(Value value); // push_back()
  void freeValueArray();             // clear() 

};

  bool valuesEqual(Value a, Value b);
  void printValue(Value value);

#endif
