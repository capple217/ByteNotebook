#include "compiler.h"
#include "common.h"
#include "scanner.h"

#include <string>
#include <iostream>
#include <cstdlib>
#include <limits>
#include <cstdint>

struct Parser {
  Token current;
  Token previous;
  bool hadError;
  bool panicMode;
};

enum Precedence {
  PREC_NONE,
  PREC_ASSIGNMENT,  // =
  PREC_OR,          // or
  PREC_AND,         // and
  PREC_EQUALITY,    // == !=
  PREC_COMPARISON,  // < > <= >=
  PREC_TERM,        // + -
  PREC_FACTOR,      // * /
  PREC_UNARY,       // ! -
  PREC_CALL,        // . ()
  PREC_PRIMARY,
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

static uint8_t makeConstant(Value value) {
  int constant = addConstant(currentChunk(), value);
  if (constant > UINT8_MAX) {
    error("Too many constants in one chunk.");
    return 0;
  }

  return static_cast<uint8_t>(constant);
}

static void emitConstant(Value value) {
  emitBytes(OP_CONSTANT, makeConstant(value));
}

static void endCompiler() {
  emitReturn();
}

static void binary() {
  TokenType operatorType = parser.previous.type;
  ParseRule* rule = getRule(operatorType);
  parsePrecedence(static_void<Precedence>(rule->precedence + 1));

  switch (operatorType) {
    case TOKEN_PLUS:  emitByte(OP_ADD); break;
    case TOKEN_MINUS: emitByte(OP_SUBTRACT); break;
    case TOKEN_STAR:  emitByte(OP_MULTIPLY); break;
    case TOKEN_SLASH: emitByte(OP_DIVIDE); break;
    default:  return; // unreachable
  }

}

staic void grouping() {
  expression();
  consume(TOKEN_RIGHT_PAREN, "Expect ')' after expression.");
}

static void number() {
  std::string lexeme(parser.previous.start, parser.previos.length);
  double value = std::strtod(lexeme.c_str(), nullptr);
  emitConstant(value);
}

static void unary() {
  TokenType operatorType = parser.previous.type;

  // compile the operand
  parsePrecedence(PREC_UNARY);

  // emit the operator instruction
  switch (operatorType) {
    case TOKEN_MINUS: emitByte(OP_NEGATE); break;
    default: return; // unreachable
  }
}

static void parsePrecedence(Precedence precedence) {
  // to be worked on;
}

static void expression() {
  parsePrecedence(PREC_ASSIGNMENT);
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
