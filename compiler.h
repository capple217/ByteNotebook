#ifndef compiler_h
#define compiler_h

#include "object.h"
#include "vm.h"

#include <string>

bool compile(const std::string source, Chunk* chunk);

#endif
