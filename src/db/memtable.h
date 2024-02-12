#ifndef MXCDB_MEMTABLE_H_
#define MXCDB_MEMTABLE_H_
#include <memory.h>

#include <memory>
#include <string_view>

#include "../util/arena.h"
#include "../util/cache.h"
#include "../util/key.h"
#include "../util/skiplistpg.h"
#include "block.h"
#include "filterblock.h"
namespace mxcdb {
class Tablebuilder;
class Memtable {
public:
  Memtable();
  ~Memtable() = default;

  Memtable(const Memtable &) = delete;
  Memtable &operator=(const Memtable &) = delete;
  void FindShortestSeparator(std::string *start, std::string_view limit);
  void FindShortSuccessor(std::string *key);
  uint32_t ApproximateMemoryUsage();
  // Iterator* NewIterator(); //TODO?
  void Add(SequenceNum seq, Valuetype type, std::string_view key,
           std::string_view value);
  bool Get(const Lookey &key, std::string *value, State *s);
  State Flushlevel0fromskip(FileMate &meta,
                            std::unique_ptr<Tablebuilder> &builder);

private:
  friend class Tablebuilder;
  std::string tmp;
  std::shared_ptr<Arena> arena;
  std::unique_ptr<Skiplist> table;
};
class Tablebuilder {
public:
  explicit Tablebuilder(const Options &opt, std::shared_ptr<WritableFile> &f)
      : options(opt), indexoptions(opt), file(f), offset(0),
        data_block(&options), index_block(&indexoptions), num_entries(0),
        closed(false), filter_block(std::make_unique<FilterBlockbuilder>()),
        pending_index_entry(false) {
    indexoptions.block_restart_interval = 1;
  }
  void Add(const InternalKey &key, const std::string &val);
  void Flush();
  void WriteBlock(Blockbuilder *block, BlockHandle *handle);
  void WriteRawBlock(const std::string_view &block_contents,
                     CompressionType type, BlockHandle *handle);
  int64_t Size() { return offset; }

private:
  Options options;
  Options indexoptions;
  std::shared_ptr<WritableFile> file; // 要生成的.sst文件
  uint64_t offset;                    // 累加每个Data Block的偏移量
  State state;
  Blockbuilder data_block;
  Blockbuilder index_block;
  std::string last_key;
  int64_t num_entries; //.sst文件中存储的所有记录总数。
  bool closed;
  std::unique_ptr<FilterBlockbuilder> filter_block;

  bool pending_index_entry; // 当一个Data Block被写入到.sst文件时，为true
  BlockHandle pending_handle; // Handle to add to index block

  std::string compressed_output; // Data Block的block_data字段压缩后的结果
};

} // namespace mxcdb
#endif