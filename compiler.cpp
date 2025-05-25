#include "compiler.h"
#include "common.h"
#include "scanner.h"

#include <string>
#include <iostream>
#include <cstdlib>

struct Parser {
  Token current;
  Token previous;
  bool hadError;
  bool panicMode;
};

Parser parser;
Chunk* compilingChunk;

static Chunk* currentChunk() {
  return compilingChunk;
}

static void ErrorAt(Token* token, const std::string& message) {
  if (parser.panicMode) return;
  parser.panicMode = true
  std::cerr << "[line " << token->line << "] Error";

  if (token->type == TOKEN_EOF) {
    std::cerr << " at end";
  }
  else if (token->type == TOKEN_ERROR) {
    // nothing
  }
  else {
    std::cerr << " at '" << std::string(token->start, token->length) << "'";
  }

  std::cerr << ": " << message << std::endl;
  parser.hadError = true;
}

static void error(const std::string& messsage) {
  errorAt(&parser.previous, message);
}

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

static void consume(TokenType type, const std::string& message) {
  if (parser.current.type == type) {
    advance();
    return;
  }

  errorAtCurrent(message);
}

static void emitByte(uint8_t byte) {
  (*currentChunk()).writeChunk(byte, parser.previous.line);             // Can maybe be rewritten as currentChunk()->writeChunk(...);
}

static void emitBytes(uint8_t byte1, uint8_t byte2) {
  emitByte(byte1);
  emitByte(byte2);
}

static void emitReturn() {
  emitByte(OP_RETURN);
}

static void endCompiler() {
  emitReturn();
}

bool compile(const std::string source, Chunk* chunk) {
  initScanner(source);
  compilingChunk = chunk;

  parser.hadError = false;
  parser.panicMode = false;

  advance();
  expression();
  consume(TOKEN_EOF, "Expect end of expresssion.");

  endCompiler();
  return !parser.hadError;
}
