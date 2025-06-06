#ifndef object_h
#define object_h

#include "common.h"
#include "memory.h"

struct Value;
#include "value.h"

#include <vector>
#include <string>


#define OBJ_TYPE(value)   (AS_OBJ(value)->type)
#define IS_STRING(value)  isObjType(value, OBJ_STRING)

#define AS_STRING(value)   static_cast<ObjString*>(AS_OBJ(value))
#define AS_CSTRING(value)  (AS_STRING(value)->chars.data())

enum ObjType {        // During revisition, want to consider scoped enums
  OBJ_STRING,

};

struct Obj {
  ObjType type;
  Obj* next;
};

struct ObjString : Obj {
  int length;
  std::vector<char> chars;
};

ObjString* takeString(std::string&& chars, int length); 

ObjString* copyString(const char* chars, int length); 
void printObject(Value value);

static inline bool isObjType(const Value& value, ObjType type) {  // macro issues bc of include file issues for value
  return IS_OBJ(value) && AS_OBJ(value)->type == type;
}

#endif
