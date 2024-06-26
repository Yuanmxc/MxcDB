#ifndef MXCDB_WRITE_BATCH_H_
#define MXCDB_WRITE_BATCH_H_
#include <string>
#include <string_view>

#include "../util/common.h"
namespace mxcdb {

class Memtable; //

class WriteBatch {
public:
  WriteBatch() { Clear(); };
  ~WriteBatch() = default;
  WriteBatch(const WriteBatch &) = default;
  WriteBatch &operator=(const WriteBatch &) = default;
  void Put(std::string_view key, std::string_view value);
  void Delete(std::string_view key);
  void Clear();
  void Append(WriteBatch *source);
  // State Iterate(Handler* handler) const;
  uint32_t mateSize() const { return mate.size(); }
  // 返回条目数
  int Count();
  // 修改条目数
  void SetCount(int n);

  SequenceNum Sequence() { return SequenceNum(DecodeFixed64(mate.data())); }
  void SetSequence(SequenceNum seq);

  std::string_view Contents() { return std::string_view(mate); }
  uint32_t ByteSize() { return mate.size(); }

  void SetContents(const std::string_view &contents);

  State InsertInto(std::shared_ptr<Memtable> memtable);

private:
  std::string mate;
  SequenceNum seq;
};
} // namespace mxcdb
#endif