#include "debug.h"
#include "value.h"

#include <cstdlib>
#include <iostream>
#include <iomanip>

void disassembleChunk(Chunk& chunk, const char* name) {
  std::cout << "== " << name << " ==" << std::endl;

  for (size_t offset = 0; offset < chunk.code.size();) {
    offset = disassembleChunk(chunk, offset);
  }
}

static int simpleInstruction(const char* name, int offset) {
  std::cout << name << std::endl;
  return offset + 1;

}

static int constantInstruction(const char* name, Chunk& chunk, int offset) {
  uint8_t constant = chunk.code[offset + 1];
  std::cout << std::left << std::setw(16) << name << std::right << std::setw(4) << static_cast<int>(constant) << " '";
  printValue(chunk.constants.values[constant]);
  std::cout << "'" << std::endl;

  return offset + 2;
}

int disassembleChunk(Chunk& chunk, int offset) {
  std::cout << std::setfill('0')  << std::setw(4) << offset << std::setfill(' ') <<  " ";

  // line info

  if (offset > 0 && chunk.lines[offset] == chunk.lines[offset - 1]) {
    std::cout << "   | ";
  }
  else {
    std::cout << std::setw(4) << chunk.lines[offset] << " ";
  }

  // OP Codes
  uint8_t instruction = chunk.code[offset];
  switch (instruction) {
    case OP_CONSTANT:
      return constantInstruction("OP_CONSTANT", chunk, offset);

    case OP_CONSTANT_LONG: // Instead of function use here, we're cluttering and filling here; prolly to be changed later;
      int byte1 = chunk.code[offset + 1];
      int byte2 = chunk.code[offset + 2];
      int byte3 = chunk.code[offset + 3];
      int constant = (byte1 << 16) | (byte2 << 8) | byte3;

      std::cout << std::left << std::setw(16) << "OP_CONSTANT_LONG" << std::right << std::setw(4) << constant << " '";
      printValue(chunk.constants[constant]);            // TODO Might have to change and fix this
      std::cout << "'" << std::endl;
      return offset + 4;

    case OP_RETURN:
      return simpleInstruction("OP_RETURN", offset);

    default:
      std::cout << "Unkown opcode " << instruction << std::endl;
      return offset + 1;                            // Would do offset++ but am lazy and forget prefix vs postfix increment
  }
}

