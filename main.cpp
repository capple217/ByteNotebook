#include "common.h"
#include "chunk.h"
#include "debug.h"
#include "vm.h"

#include <iostream>
#include <fstream>
#include <cstdlib>
#include <string>
#include <sstream>

static void repl() {
  std::string line;
  while (true) {
    std::cout << "> ";

    if (!std::getline(std::cin, line)) {
      std::cout << std::endl;
      break;
    }

    interpret(line);
  }
}

static void runFile(const std::string& path) {
  std::ifstream file(path);
  if (!file.is_open()) {
    std::cerr << "Could not open file: " << path << std::endl;
    std::exit(74);
  }

  std::stringstream buffer;
  buffer << file.rdbuf();
  std::string source = buffer.str();

  InterpretResult result = interpret(source.c_str());

  if (result == INTERPRET_COMPILE_ERROR) std::exit(65);
  if (result == INTERPRET_RUNTIME_ERROR) std::exit(70);
}                       // Theoretically, we shouldn't have issues with memory to read data since we're using std::string instead of arrays

int main(int argc, const char* argv[]) {

  initVM();           // Might just have an instantiation of the struct instead: VM vm
  
  if (argc == 1) {
    repl();
  }
  else if (argc == 2) {
  runFile(argv[1]);
  }
  else {
    std::cerr << "Usage: VM [path]\n";
    std::exit(64);
  }

  freeVM();
  return 0;
}
