#ifndef MXCDB_COMMON_H_
#define MXCDB_COMMON_H_
#include <cstddef>
#include <memory.h>
#include <string_view>

#include "spdlog/spdlog.h"
namespace mxcdb {

typedef uint64_t SequenceNum;

// 定长 fix
// 非定长 varint
// 编码
void PutFixed32(std::string *dst, uint32_t value);
void PutFixed64(std::string *dst, uint64_t value);
void PutVarint32(std::string *dst, uint32_t value);
void PutVarint64(std::string *dst, uint64_t value);
void PutLengthPrefixed(
    std::string *dst,
    const std::string_view &value); // dst： 长度(编码后) + value

// 解码
bool GetVarint32(std::string_view *input, uint32_t *value);
bool GetVarint64(std::string_view *input, uint64_t *value);
bool GetLengthPrefixed(std::string_view *input, std::string_view *result);

// 解码变长整形最基础函数。
const char *GetVarint32Ptr(const char *p, const char *limit, uint32_t *v);
const char *GetVarint64Ptr(const char *p, const char *limit, uint64_t *v);

// 获取编码后长度
int VarintLength(uint64_t v);

// 编码变长整形的最基础函数。
char *EncodeVarint32(char *dst, uint32_t value);
char *EncodeVarint64(char *dst, uint64_t value);

inline void EncodeFixed32(char *dst, uint32_t value) {
  uint8_t *const buffer = reinterpret_cast<uint8_t *>(dst);
  std::memcpy(buffer, &value, sizeof(uint32_t));
  return;
}

inline void EncodeFixed64(char *dst, uint64_t value) {
  uint8_t *const buffer = reinterpret_cast<uint8_t *>(dst);
  std::memcpy(buffer, &value, sizeof(uint64_t));
  return;
}

inline uint32_t DecodeFixed32(const char *ptr) {
  const uint8_t *const buffer = reinterpret_cast<const uint8_t *>(ptr);

  uint32_t result;
  std::memcpy(&result, buffer, sizeof(uint32_t));
}

inline uint64_t DecodeFixed64(const char *ptr) {
  const uint8_t *const buffer = reinterpret_cast<const uint8_t *>(ptr);
  uint64_t result;
  std::memcpy(&result, buffer, sizeof(uint64_t));
  return result;
}

const char *GetVarint32PtrFallback(const char *p, const char *limit,
                                   uint32_t *value);
inline const char *GetVarint32Ptr(const char *p, const char *limit,
                                  uint32_t *value) {
  if (p < limit) {
    uint32_t result = *(reinterpret_cast<const uint8_t *>(p));
    if ((result & 128) == 0) { // 字节最高 bit 为0，说明编码结束。
      *value = result;
      return p + 1;
    }
  }
  return GetVarint32PtrFallback(p, limit, value);
}
bool GetLengthPrefixed(std::string_view *input, std::string_view *result);
const char *GetLengthPrefixed(const char *p, const char *limit,
                              std::string_view *result);
const char *GetLengthPrefixedview(const char *p, const char *limit,
                                  std::string_view *result);
bool GetLengthPrefixedview(std::string_view *input, std::string_view *result);
class State {
public:
  State() : state_(nullptr) {}
  ~State();

  State(const State &rhs);
  State &operator=(const State &rhs) {
    if (state_ != nullptr) {
      delete[] state_;
      state_ = (rhs.state_ == nullptr) ? nullptr : CopyState(rhs.state_);
    }
  }

  State(State &&rhs) noexcept : state_(rhs.state_) { rhs.state_ = nullptr; }
  static State Ok() { return State(); }
  static State Notfound(const std::string &msg) {
    return State(knotfound, msg);
  }
  static State Corruption(const std::string &msg) {
    return State(kcorruption, msg);
  }
  static State NotSupported(const std::string &msg) {
    return State(knotsupported, msg);
  }
  static State InvalidArgument(const std::string &msg) {
    return State(kinvalidargument, msg);
  }
  static State IoError(const std::string &msg) { return State(kioerror, msg); }
  bool ok() const { return (state_ == nullptr); }

  bool IsNotFound() const { return code() == knotfound; }

  bool IsCorruption() const { return code() == kcorruption; }

  bool IsIOError() const { return code() == kioerror; }

  bool IsNotSupportedError() const { return code() == knotsupported; }

  bool IsInvalidArgument() const { return code() == kinvalidargument; }

  std::string ToString() const;

private:
  enum Code {
    kok = 0,
    knotfound = 1,
    kcorruption = 2,
    knotsupported = 3,
    kinvalidargument = 4,
    kioerror = 5
  };
  Code code() const {
    return (state_ == nullptr) ? kok : static_cast<Code>(state_[4]);
  }

  State(Code code, const std::string &msg);
  static const char *CopyState(const char *s);
  const char *state_;
};
} // namespace mxcdb
#endif