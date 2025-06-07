#ifndef compiler_h
#define compiler_h

#include "object.h"
#include "vm.h"
#include "scanner.h"

#include <string>

bool compile(const std::string source, Chunk* chunk);

static void expression();
static void statement();
static void declaration();
// might have to add static ParseRule* getRule(TokenType type);


static uint8_t identifierConstant(Token* name);

#endif
