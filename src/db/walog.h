#ifndef MXCDB_WALOG_H_
#define MXCDB_WALOG_H_
#include <memory.h>
#include <string_view>

#include "../util/common.h"
#include "../util/env.h"
#include "block.h"
#include "crc32c/crc32c.h"

namespace mxcdb {

enum RecordType {
  // zero is reserved for preallocated files
  kZeroType = 0,

  kFullType = 1,

  kFirstType = 2,
  kMiddleType = 3,
  kLastType = 4
};
static const int kBlockSize = 32768;
static const int kHeaderSize = 7;
// checksum (4 bytes), length (2 bytes), type (1 byte).
class Record {
public:
  Record() = default;
  ~Record() = default;

private:
};
class walWriter {
public:
  explicit walWriter(std::shared_ptr<WritableFile> file_) : block_offset(0) {
    file = file_;
    for (int i = 0; i <= kLastType; i++) {
      char t = static_cast<char>(i);
      type_crc[i] = crc32c::Crc32c(&t, 1);
    }
  }
  ~walWriter() = default;
  State Appendrecord(std::string_view str);

private:
  State Flushphyrecord(RecordType type, const char *buf_, uint32_t size);
  std::shared_ptr<WritableFile> file;
  int block_offset;
  uint32_t type_crc[kLastType + 1];
};
class walReader {
public:
  explicit walReader();
  ~walReader() = default;

private:
  std::unique_ptr<ReadFile> file;
  bool const checksum;
  char *const readbuf;
  std::string_view readbuf_;
  bool error;

  uint64_t lastrecord_offset;
  uint64_t nowoffset;
  uint64_t init_offset;
};
} // namespace mxcdb
#endif