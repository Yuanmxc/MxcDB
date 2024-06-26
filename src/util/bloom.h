#ifndef MXCDB_FILTER_H_
#define MXCDB_FILTER_H_
#include <memory>
#include <string_view>

#include "spdlog/spdlog.h"

namespace mxcdb {

class BloomFilter {
public:
  explicit BloomFilter(int bits);
  ~BloomFilter() = default;
  // 给长度为 n 的 key,创建一个过滤策略，并将策略序列化为 string ，追加到 dst
  void CreateFiler(std::string_view *key, int n,
                   std::string *filter) const; // d
  bool KeyMayMatch(const std::string_view &key, std::string_view &filter) const;

private:
  size_t bits_key;
  size_t hash_size;
};
} // namespace mxcdb
#endif
