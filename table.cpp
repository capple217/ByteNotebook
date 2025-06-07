#include "table.h"
#include "value.h"
#include "object.h"

#include <unordered_map>
#include <string>
#include <string_view>
#include <cstring>

bool Table::set(ObjString* key, Value value) {
  auto [it, inserted] = entries.emplace(key, value);
  if (!inserted) it->second = value;
  return inserted;
}

bool Table::get(ObjString* key, Value& out) {
  auto it = entries.find(key);
  if (it == entries.end()) return false;
  out = it->second;
  return true;
}

bool Table::remove(ObjString* key) {
  return entries.erase(key) > 0;
}


void Table::clear() {
  entries.clear();
}

size_t Table::size() const {
  return entries.size();
}
