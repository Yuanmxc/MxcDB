#include "memtable.h"
namespace mxcdb {
void Memtable::FindShortestSeparator(std::string *start,
                                     std::string_view limit) {}
void Memtable::FindShortSuccessor(std::string *key) {}
size_t Memtable::ApproximateMemoryUsage();
// Iterator* NewIterator(); //TODO?
void Memtable::Add(SequenceNum seq, Valuetype type, std::string_view key,
                   std::string_view value) {}
bool Memtable::Get(const LookupKey &key, std::string *value, State *s) {}
} // namespace mxcdb