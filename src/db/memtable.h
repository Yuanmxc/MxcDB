#ifndef MXCDB_MEMTABLE_H_
#define MXCDB_MEMTABLE_H_
#include <memory.h>
#include <string_view>

#include "../util/arena.h"
#include "../util/key.h"
#include "../util/skiplistpg.h"
namespace mxcdb {
class Memtable {
public:
  explicit Memtable();
  ~Memtable();

  Memtable(const Memtable &) = delete;
  Memtable &operator=(const Memtable &) = delete;
  void FindShortestSeparator(std::string *start, std::string_view limit);
  void FindShortSuccessor(std::string *key);
  size_t ApproximateMemoryUsage();
  // Iterator* NewIterator(); //TODO?
  void Add(SequenceNum seq, Valuetype type, std::string_view key,
           std::string_view value);
  bool Get(const LookupKey &key, std::string *value, State *s);

private:
  std::unique_ptr<Skiplist> table;
  std::unique_ptr<Arena> arena;
};

} // namespace mxcdb
#endif