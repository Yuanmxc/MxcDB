#include "writebatch.h"

#include "../util/key.h"
#include "memtable.h"
#include "../util/common.h"
namespace mxcdb {
// 前8字节是该WriteBatch的SequenceNumber，后4字节是该WriteBatch中Entry的数量
static const uint32_t Headsize = 12;
void WriteBatch::Clear() {
  mate.clear();
  mate.resize(Headsize);
}
void WriteBatch::Put(std::string_view key, std::string_view value) {
  SetCount(Count() + 1);
  mate.push_back(static_cast<char>(kTypeValue));
  PutLengthPrefixedview(&mate, key);
  PutLengthPrefixedview(&mate, value);
}

void WriteBatch::Delete(std::string_view key) {
  SetCount(Count() + 1);
  mate.push_back(static_cast<char>(kTypeDeletion));
  PutLengthPrefixedview(&mate, key);
}

void WriteBatch::Append(WriteBatch *source) {
  SetCount(source->Count() + Count());
  mate.append(source->mate.data() + Headsize);
}

int WriteBatch::Count() { return DecodeFixed32(mate.data() + 8); }

void WriteBatch::SetCount(int n) { EncodeFixed32(&mate[8], n); }
void WriteBatch::SetSequence(SequenceNum seq) {
  EncodeFixed64(mate.data(), seq);
}

State WriteBatch::InsertInto(std::shared_ptr<Memtable> memtable) {
  std::string_view ptr(mate);
  SequenceNum now_seq = Sequence();
  int now_cnt = 0;
  if (ptr.size() < Headsize) {
    mlog->error("malformed WriteBatch (too small)");
  }
  ptr.remove_prefix(Headsize);
  std::string_view key, value;
  char type;
  while (!ptr.empty()) {
    now_cnt++;
    type = ptr[0];
    ptr.remove_prefix(1);
    switch (type) {
    case kTypeValue:
      if (GetLengthPrefixedview(&ptr, &key) &&
          GetLengthPrefixedview(&ptr, &value)) {
        memtable->Add(now_seq, kTypeValue, key, value);
        mlog->info("memtable add Seq:{} Type:{} Key:{} Value:{}", now_seq,
                   kTypeValue, key, value);
      } else {
        mlog->error("bad WriteBatch Put");
        return State::Corruption();
      }
      break;
    case kTypeDeletion:
      if (GetLengthPrefixedview(&ptr, &key)) {
        memtable->Add(now_seq, kTypeDeletion, key, std::string_view());
        mlog->trace("memtable add Seq:{} Type:{} Key:{} Value:{}", now_seq,
                    kTypeDeletion, key, value);
      } else {
        mlog->error("bad WriteBatch Del");
        return State::Corruption();
      }
      break;
    default:
      mlog->error("unknown WriteBatch type");
      return State::Corruption();
    }
    seq++;
  }
  if (now_cnt != Count()) {
    mlog->error("WriteBatch has wrong count has {} should {}", now_cnt,
                Count());
    return State::Corruption();
  } else {
    return State::Ok();
  }
}
void WriteBatch::SetContents(const std::string_view &contents) {
  assert(contents.size() >= kHeader);
  mate.assign(contents.data(), contents.size());
}
} // namespace mxcdb