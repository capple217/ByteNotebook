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

void initScanner(const std::string& source) {
  scanner.start = &source;
  scanner.current = &source;
  scanner.line = 1;
}

static bool isDigit(char c) {
  return c >= '0' && c <= '9';
}

static bool isAtEnd() {
  return *scanner.current == '\0';
}

static char advance() {
  scanner.current++;
  return scanner.current[-1];               // Unfamiliar with this syntax might have to come back here and fix
}

static char peek() {
  return *scanner.current;
}

static char peekNext() {
  if (isAtEnd()) return '\0';
  return scanner.current[1];
}

static bool match(char expected) {
  if (isAtEnd()) return false;
  if (*scanner.current != expected) return false;
  scanner.current++;
  return true;
} 

static Token makeToken(TokenType type) {
  Token token;
  token.type = type;
  token.start = scanner.start;
  token.length = static_cast<int>(scanner.current - scanner.start);
  token.line = scanner.line;
  return token;
}

static Token errorToken(const std::string& message) {
  Token token;
  token.type = TOKEN_ERROR;
  token.start = &message;
  token.length = message.size();
  token.line = scanner.line;
  return token;
}

static void skipWhitespace() {
  while (true) {
    char c = peek();
    switch (c) {
      case ' ':
      case '\r':
      case '\t':
        advance();
        break;
      case '\n':
        scanner.line++;
        advance();
        break;
      case '/':
        if (peekNext == '/') {
          while (peek() != '\n' && !isAtEnd()) advance();
        }
        else {
          return;
        }
        break;
      default:
        return;
    }
  }
}

static Token number() {
  while (isDigit(peek())) advance();

  // look for a fractional part
  if (peek() == '.' && isDigit(peekNext())) {
    advance();

    while (isDigit(peek())) advance();
  }

  return makeToken(TOKEN_NUMBER);
}

static Token string() {
  while (peek() != '"' && !isAtEnd()) {
    if (peek == '\n') scanner.line++;
    advance();
  }

  if (isAtEnd()) return errorToken("Unterminated string.");

  // Closing quote
  advance();
  return makeToken(TOKEN_STRING);
}

Token scanToken() {                             // Here is where we scan over each "lexeme" and turn it into tokens for our compiler to understand
  skipWhitespace();                             // First skip over any white space before reading actual lexemes and characters
  scanner.start = scanner.current;

  if (isAtEnd()) return makeToken(TOKEN_EOF);

  char c = advance();
  if (isDigit(c)) return number();

  switch (c) {
    case '(': return makeToken(TOKEN_LEFT_PAREN);
    case ')': return makeToken(TOKEN_RIGHT_PAREN);
    case '{': return makeToken(TOKEN_LEFT_BRACE);
    case '}': return makeToken(TOKEN_RIGHT_BRACE);
    case ';': return makeToken(TOKEN_SEMICOLON);
    case ',': return makeToken(TOKEN_COMMA);
    case '.': return makeToken(TOKEN_DOT);
    case '-': return makeToken(TOKEN_MINUS);
    case '+': return makeToken(TOKEN_PLUS);
    case '/': return makeToken(TOKEN_SLASH);
    case '*': return makeToken(TOKEN_STAR);
    case '!':
      return makeToken(
          match('=') ? TOKEN_BANG_EQUAL : TOKEN_BANG);
    case '=':
      return makeToken(
          match('=') ? TOKEN_EQUAL_EQUAL : TOKEN_EQUAL);
    case '<':
      return makeToken(
          match('=') ? TOKEN_LESS_EQUAL : TOKEN_LESS);
    case '>':
      return makeToken(
          match('=') ? TOKEN_GREATER_EQUAL : TOKEN_GREATER);
    case '"': retirm string();
  }

  return errorToken("Unexpected character.");
}
