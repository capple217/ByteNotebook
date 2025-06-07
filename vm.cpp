#include "vm.h"
#include "chunk.h"
#include "common.h"
#include "value.h"
#include "debug.h"
#include "compiler.h"
#include "object.h"
#include "memory.h"

#include <iostream>
#include <vector>
#include <format>
#include <string>
#include <cstdarg>

VM vm;

static void resetStack() {
  vm.stack.clear();
}

static void runtimeError(const std::string& format) { // MIGHT have to revisit to ensure all working parts 

  std::cerr << format << "\n";

  size_t instruction = static_cast<size_t>(std::distance(vm.chunk->code.data(), vm.ip)) - 1;
  int line = vm.chunk->lines[instruction];
  std::cerr << "[line " << line << "] in script\n";
  resetStack();
}

void initVM() {
  resetStack();
  vm.objects = nullptr;
}

void freeVM() {
  vm.globals.clear();
  vm.strings.clear();
  freeObjects();
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

static bool isFalsey(Value value) {
  return IS_NIL(value) || (IS_BOOL(value) && !AS_BOOL(value));
}

static void concatenate() {     // Have to recheck heap alloc w this
  ObjString* b = AS_STRING(pop());
  ObjString* a = AS_STRING(pop());

  std::string combo;
  combo.reserve(a->chars.size() + b->chars.size());
  combo.append(a->chars.data(), a->chars.size());
  combo.append(b->chars.data(), b->chars.size());

  ObjString* result = takeString(std::move(combo), static_cast<int>(combo.size()));
  
  push(OBJ_VAL(result));
}

static InterpretResult run() {                    // Main function the VM will be running the whole time; endless loop till termination (oxymoronic)
#define READ_BYTE() (*vm.ip++)
#define READ_CONSTANT() (vm.chunk->constants.values[READ_BYTE()])
#define READ_STRING() AS_STRING(READ_CONSTANT())
#define BINARY_OP(valueType, op) \
  do { \
    if (!IS_NUMBER(peek(0)) || !IS_NUMBER(peek(1))) { \
      runtimeError("Operands must be numbers."); \
      return INTERPRET_RUNTIME_ERROR; \
    } \
    double b = AS_NUMBER(pop()); \
    double a = AS_NUMBER(pop()); \
    push(valueType(a op b)); \
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

      case OP_NIL:    push(NIL_VAL); break;
      case OP_TRUE:   push(BOOL_VAL(true)); break;
      case OP_FALSE:  push(BOOL_VAL(false)); break;

      case OP_POP:    pop(); break;

      case OP_GET_GLOBAL: {
        ObjString* name = READ_STRING();
        Value value;
        if (!vm.globals.get(name, value)) {
          std::string varName{name->chars.data(), static_cast<size_t>(name->length)};
          std::string msg = "Undefined variable '" + varName + "'.";
          runtimeError(msg);
          return INTERPRET_RUNTIME_ERROR;
        }
        push(value);
        break;
      }

      case OP_DEFINE_GLOBAL: {
        ObjString* name = READ_STRING();
        vm.globals.set(name, peek(0));
        pop();
        break;
      }

      case OP_SET_GLOBAL: {
        ObjString* name = READ_STRING();
        if (vm.globals.set(name, peek(0))) {
          vm.globals.remove(name);
          std::string varName{name->chars.data(), static_cast<size_t>(name->length)};
          std::string msg = "Undefined variable '" + varName + "'.";
          runtimeError(msg);
        }
        break;
      }

      case OP_EQUAL: {
          Value b = pop();
          Value a = pop();
          push(BOOL_VAL(valuesEqual(a, b)));
           break;
        }

      case OP_GREATER:  BINARY_OP(BOOL_VAL, >); break;
      case OP_LESS:     BINARY_OP(BOOL_VAL, <); break;

      case OP_ADD: {
          if (IS_STRING(peek(0)) && IS_STRING(peek(1))) {
            concatenate();
          }
          else if (IS_NUMBER(peek(0)) && IS_NUMBER(peek(1))) {
            double b = AS_NUMBER(pop());
            double a = AS_NUMBER(pop());
            push(NUMBER_VAL(a + b));
          }
          else {
            runtimeError("Operands must be two numbers or two strings");
            return INTERPRET_RUNTIME_ERROR;
          }
        }
        break;

      case OP_SUBTRACT:
        BINARY_OP(NUMBER_VAL, -);
        break;

      case OP_MULTIPLY:
        BINARY_OP(NUMBER_VAL, *);
        break;

      case OP_DIVIDE:
        BINARY_OP(NUMBER_VAL, /);
        break;

      case OP_NOT:
        push(BOOL_VAL(isFalsey(pop())));
        break;

      case OP_NEGATE:
        if (!IS_NUMBER(peek(0))) {
          runtimeError("Operand must be a number");
          return INTERPRET_RUNTIME_ERROR;
        }
        push(NUMBER_VAL(-AS_NUMBER(pop())));
        break;

      case OP_PRINT: {
        printValue(pop());
        std::cout << "\n";
        break;
        }

      case OP_RETURN: {
        // Exit interpreter
        return INTERPRET_OK;
      }
    }
  }

#undef READ_BYTE
#undef READ_CONSTANT
#undef READ_STRING
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
