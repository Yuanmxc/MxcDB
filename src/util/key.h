#ifndef MXCDB_KEY_H_
#define MXCDB_KEY_H_
#include <cstddef>
#include <cstdint>
#include <string.h>
#include <string>
#include <string_view>

#include "common.h"
#include "key.h"
#include "skiplist.h"
namespace mxcdb {
typedef uint64_t SequenceNum;
enum Valuetype {
  kTypeDeletion = 0X0,
  kTypeValue = 0x1,
};
using keycomper = std::function<bool(std::string_view, std::string_view)>;
static const SequenceNum kMaxSequenceNumber = ((0x1ull << 56) - 1); // max seq
static std::string_view ExtractUserKey(std::string_view internal_key) {
  assert(internal_key.size() >= 8);
  return std::string_view(internal_key.data(), internal_key.size() - 8);
}
static int cmp(std::string_view a, std::string_view b) { // inernalkey cmp
  int r = strcmp(ExtractUserKey(a).data(), ExtractUserKey(b).data());
  if (r == 0) {
    const uint64_t anum = DecodeFixed64(a.data() + a.size() - 8);
    const uint64_t bnum = DecodeFixed64(b.data() + b.size() - 8);
    if (anum > bnum) {
      r = -1;
    } else if (anum < bnum) {
      r = +1;
    }
  }
  return r;
}
class InternalKey {
public:
  explicit InternalKey(std::string_view str, SequenceNum num, Valuetype type) {
    Key.append(str.data(), str.size());
    PutFixed64(&Key, parser(num, type));
  }
  ~InternalKey();
  std::string_view getview() { return std::string_view(Key); }
  uint64_t parser(SequenceNum num, Valuetype type);

private:
  std::string Key;
};
class SkiplistKey { // for skiplist
public:
  explicit SkiplistKey(uint32_t intersize, InternalKey interkey_,
                       std::string_view value)
      : internalsize(intersize), interkey(interkey_), mate(value) {}
  ~SkiplistKey() = default;
  std::string_view getview() { return interkey.getview(); }
  SkiplistKey &operator=(const SkiplistKey &a) {
    internalsize = a.internalsize;
    interkey = a.interkey;
    mate = a.mate;
  }

private:
  uint32_t internalsize;
  InternalKey interkey;
  std::string_view mate;
};
static bool compar(const InternalKey &a, const InternalKey &b) {}
class LookupKey {
public:
private:
};
} // namespace mxcdb
#endif