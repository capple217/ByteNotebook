#ifndef object_h
#define #object_h

#include "common.h"
#include "value.h"

#include <vector>

#define OBJ_TYPE(value)   (AS_OBJ(value)->type)
#define IS_STRING(value)  isObjType(value, OBJ_STRING)

#define AS_STRING(value)  ((ObjString*)AS_OBJ(value))             // Takes a Value expected to contain a pointer to a valid ObjString on the heap
#define AS_CSTRING(value) (((ObjString*)AS_OBJ(value))->chars)    // Takes this one step further an returns the char array since often that's what we want

enum ObjType {        // During revisition, want to consider scoped enums
  OBJ_STRING,

};


struct Obj{
  ObjType type;

};

struct ObjString {              // ObjType in struct Obj shares the memory the Obj obect takes in ObjString
  Obj obj;
  int length;
  std::vector<char> chars;

};

ObjString* copyString(const char* chars, int length); 
void printObject(Value value);

static inline bool isObjType(Value value, ObjType type) {
  return IS_OBJ(value) && AS_OBJ(value)->type == type;
}

#endif
