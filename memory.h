#ifndef memory_h
#define memory_h

#include <cstddef>

void* reallocate(void* pointer, std::size_t oldSize, std::size_t newSize);

void freeObjects();

#endif // !memory_h
