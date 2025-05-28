#include "vm.h"
#include "common.h"
#include "value.h"
#include "debug.h"
#include "compiler.h"

#include <iostream>
#include <vector>

VM vm;

static void resetStack() {
  vm.stack.clear();
}

static void runtimeError(const std::string& format) { // FINISH UP NOT DONE AT ALL {18.3.1}

}

void initVM() {
  resetStack();

}

void freeVM() {

}

void push(Value value) {
  vm.stack.push_back(value);
}

Value pop() {
  Value val = vm.stack.back();
  vm.stack.pop_back();
  return val;
}

static Value peek(int distance) {
  return vm.stack[vm.stack.size() - 1 - distance];
}

static InterpretResult run() {                    // Main function the VM will be running the whole time; endless loop till termination (oxymoronic)
#define READ_BYTE() (*vm.ip++)
#define READ_CONSTANT() (vm.chunk->constants.values[READ_BYTE()])
#define BINARY_OP(op) \
  do {  \
    double b = pop(); \
    double a = pop(); \
    push(a op b); \
  } while (false);                          // Really cool use of macros, want to reuse another time
  
  while (true) {
#ifdef DEBUG_TRACE_EXECUTION
    std::cout << "          ";
    for (Value val : vm.stack) {
      std::cout << "[ ";
      printValue(val);
      std::cout << " ]";
    }
    std::cout << std::endl;


    disassembleInstruction(*vm.chunk, static_cast<int>(vm.ip - vm.chunk->code.data()));         
#endif

    uint8_t instruction;
    switch (instruction = READ_BYTE()) {
      case OP_CONSTANT: {
        Value constant = READ_CONSTANT();
        push(constant);
        break;
      }

      case OP_ADD:
        BINARY_OP(+);
        break;

      case OP_SUBTRACT:
        BINARY_OP(-);
        break;

      case OP_MULTIPLY:
        BINARY_OP(*);
        break;

      case OP_DIVIDE:
        BINARY_OP(/);
        break;

      case OP_NEGATE:
        if (!IS_NUMBER(peek(0))) {
          runtimeError("Operand must be a number");
          return INTERPRET_RUNTIME_ERROR;
        }
        push(NUMBER_VAL(-AS_NUMBER(pop())));
        break;

      case OP_RETURN: {
        printValue(pop());
        std::cout << std::endl;
        return INTERPRET_OK;
      }
    }
  }

#undef READ_BYTE
#undef READ_CONSTANT
#undef BINARY_OP
}

InterpretResult interpret(const std::string source) {
  Chunk chunk;

  if (!compile(source, &chunk)) {
    chunk.freeChunk();
    return INTERPRET_COMPILE_ERROR;
  }

  vm.chunk = &chunk;
  vm.ip = vm.chunk->code.data();

  InterpretResult result = run();

  chunk.freeChunk();
  return result;
}
