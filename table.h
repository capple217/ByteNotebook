#ifndef table_h
#define table_h

#include "common.h"
#include "value.h"
#include "object.h"

#include <unordered_map>
#include <string>
#include <string_view>
#include <cstring>
#include <algorithm>

// Deviating heavily from original C version thanks to unordered_map already existing

class Table {
public:

    class Hash {
    public:
        using is_transparent = void;

        size_t operator()(ObjString* s) const noexcept {
          return std::hash<std::string_view>()(
            std::string_view(s->chars.data(), s->length)
          );
        }

        size_t operator()(std::string_view sv) const noexcept {
          return std::hash<std::string_view>()(sv);
        }
    };

    class Eq {
    public:
        using is_transparent = void;

        bool operator()(ObjString* a, ObjString* b) const noexcept {
          if (a->length != b->length) return false;
          return std::equal(a->chars.begin(), a->chars.end(), b->chars.begin());
        }

        bool operator()(ObjString* a, std::string_view sv) const noexcept {
          if (a->length != int(sv.size())) return false;
          return std::equal(a->chars.begin(), a->chars.end(), sv.begin());
        }

        bool operator()(std::string_view sv, ObjString* b) const noexcept {
          return operator()(b, sv);
        }
    };

    using MapType = std::unordered_map<ObjString*, Value, Hash, Eq>;
    MapType entries;

    bool set(ObjString* key, Value value);

    bool get(ObjString* key, Value& out);

    bool remove(ObjString* key);

    void clear();

    size_t size() const;
};

#endif
