#include "compiler.h"
#include "common.h"
#include "scanner.h"

#include <string>
#include <iostream>

struct Parser {
  Token current;
  Token previous;
};

Parser parser;

static void errorAtCurrent(const std::string& message) {
  errorAt(&parser.current, message);
}

static void advance() {
  parser.previous = parser.current;

  while (true) {
    parset.current = scanToken();
    if (parset.current.type != TOKEN_ERROR) break;

    errorAtCurrent(parser.current.start);
  }
}

bool compile(const std::string source, Chunk* chunk) {
  initScanner(source);
  advance();
  expression();
  consume(TOKEN_EOF, "Expect end of expresssion.");
}
