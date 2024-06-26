#ifndef MXCDB_CACHE_H_
#define MXCDB_CACHE_H_
#include <memory.h>

#include <list>
#include <map>
#include <mutex>
#include <string>
#include <string_view>
#include <unordered_map>

#include "../util/env.h"
#include "../util/options.h"
#include "iterator.h"
#include "sstable.h"
namespace mxcdb {
struct CacheHandle {
  std::shared_ptr<void> str;
};
using kvpair = std::pair<std::string_view, CacheHandle>;
class LruCache {
public:
  LruCache() : size(0), use(0) {}
  ~LruCache() = default;
  void SetCapacity(size_t capacity) { size = capacity; }
  void Erase(std::string_view ekey);
  CacheHandle *Insert(kvpair &pair);
  CacheHandle *Lookup(std::string_view &key);
  size_t Getsize() const { return cacheList.size() * (sizeof(kvpair) + 8); };

private:
  uint64_t size;
  uint64_t use;
  std::mutex mutex;
  std::list<std::shared_ptr<kvpair>> cacheList;
  std::unordered_map<std::string_view,
                     std::list<std::shared_ptr<kvpair>>::iterator>
      cacheMap;
};
class ShareCache {
public:
  explicit ShareCache(uint64_t size) : last_id(0) {
    for (int s = 0; s < 16; s++) {
      sharecache[s] = std::make_unique<LruCache>();
      sharecache[s]->SetCapacity(size / 16);
    }
  }
  ~ShareCache() {}
  CacheHandle *Insert(std::string_view &key, std::shared_ptr<void> value);
  CacheHandle *Lookup(std::string_view &key);
  void Erase(std::string_view ekey);
  // void* Value(CacheHandle* handle) {
  //   return reinterpret_cast<CacheHandle*>(handle)->;
  // }
  uint64_t NewId() {
    std::unique_lock<std::mutex>(mutex);
    return ++(last_id);
  }
  size_t Getsize();

private:
  std::unique_ptr<LruCache> sharecache[16];
  std::mutex mutex;
  int last_id;
};
class TableCache { //(SSTable.file_number)->(TableAndFile*)
public:
  explicit TableCache(std::string_view dbname, const Options *opt);
  ~TableCache() { delete env; }
  std::shared_ptr<Iterator>
  NewIterator(const ReadOptions &options, uint64_t file_number,
              uint64_t file_size, std::shared_ptr<Table> tableptr = nullptr);
  State Get(const ReadOptions &readopt, uint64_t file_num, uint64_t file_size,
            std::string_view key, void *arg,
            void (*handle_rul)(void *, std::string_view a, std::string_view b));
  void Evict(uint64_t file_number);
  void *Value(CacheHandle *handle);

private:
  State FindTable(uint64_t file_num, uint64_t file_size, CacheHandle **handle);

  PosixEnv *env;
  std::string dbname;
  const Options *const opt;
  std::unique_ptr<ShareCache> cache;
};
} // namespace mxcdb
#endif
