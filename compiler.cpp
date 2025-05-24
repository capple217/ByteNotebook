#include "compiler.h"
#include "common.h"
#include "scanner.h"

#include <string>
#include <iostream>

void compile(const std::string source) {
  initScanner(source);
  int line = -1;
  while (true) {
    Token token = scanToken();
    if (token.line != line) {
      std::cout << std::setw(4) << token.line << " ";
      line = token.line;
    }
    else {
      std::cout << "   | ";
    }
    std::cout << std::setw(2) << token.type << " '" << std::string(token.start, token.length) << "'\n";

    if (token.type == TOKEN_EOF) break;
  }
}
