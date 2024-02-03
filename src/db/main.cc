#include <assert.h>
#include <iostream>
#include <string>

#include "../util/common.h"
#include "db.h"
#include "loginit.h"
#include "writebatch.h"
#define SPDLOG_ACTIVE_LEVEL                                                    \
  SPDLOG_LEVEL_TRACE // 必须定义这个宏,才能输出文件名和行号
int main() {
  mxcdb::DB *db;
  mxcdb::Options opt;
  spdlog::set_level(spdlog::level::debug);
  spdlog::set_pattern("[%Y-%m-%d %H:%M:%S.%e][thread %t][%@,%!][%l] : %v");
  mxcdb::State s = mxcdb::DBImpl::Open(opt, "/tmp/testdb", &db);
  assert(s.ok());

  // write key1,value1
  std::string key = "key";
  std::string value = "value";

  s = db->Put(mxcdb::WriteOptions(), key, value);
  assert(s.ok());
  return 0;
}
