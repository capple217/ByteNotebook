# Compiler and flags
CXX = g++
CXXFLAGS = -std=c++17 -Wall -Wextra -O2

# Target name
TARGET = ByteNotebook

# Source files
SRCS = main.cpp chunk.cpp debug.cpp value.cpp vm.cpp compiler.cpp scanner.cpp object.cpp
# Object files 
OBJS = $(SRCS:.cpp=.o)

# Header dependencies (for clarity, not used directly unless we set up dependencies)
HDRS = common.h chunk.h debug.h value.h vm.h compiler.h scanner.h object.h

# Default target
all: $(TARGET)

# Link the object files into the final binary
$(TARGET): $(OBJS)
	$(CXX) $(CXXFLAGS) -o $@ $(OBJS)

# Rule to compile .cpp to .o
%.o: %.cpp $(HDRS)
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Clean target
clean:
	rm -f $(TARGET) $(OBJS)

.PHONY: all clean

