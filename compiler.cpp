#include "compiler.h"
#include "common.h"
#include "scanner.h"

#include <string>
#include <iostream>

bool compile(const std::string source, Chunk* chunk) {
  initScanner(source);
  advance();
  expression();
  consume(TOKEN_EOF, "Expect end of expresssion.");
}
