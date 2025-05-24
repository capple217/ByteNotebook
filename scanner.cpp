#include "scanner.h"
#include "common.h"

#include <string>
#include <cstdio>

struct Scanner {
  const char* start;
  const char* current;
  int line;
}

Scanner scanner;
