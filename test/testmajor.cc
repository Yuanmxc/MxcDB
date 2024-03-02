#include <assert.h>

#include <cstddef>
#include <iostream>
#include <string>

#include "src/db/db.h"
#include "src/util/common.h"
#include "src/util/loger.h"
#include "gtest/gtest.h"
using namespace mxcdb;
TEST(testmemtable, test0) {
  mxcdb::DB *db;
  mxcdb::Options opt;
  mxcdb::State s = mxcdb::DBImpl::Open(opt, "/tmp/testdb", &db);
  assert(s.ok());

  // write key1,value1
  std::string key = "key0";
  std::string value = "value0";
  std::string value1 = "value0";
  size_t p = 0;
  for (int i = 0; i < 200; i++) {
    s = db->Put(mxcdb::WriteOptions(), key, value);
    key.replace(3, std::to_string(i).size(), std::to_string(i));
    value.replace(5, std::to_string(i).size(), std::to_string(i));
    p += key.size() + value.size() + 8 + VarintLength(key.size()) +
         VarintLength(value.size());
    EXPECT_TRUE(s.ok());
  }
  mlog->info("All size = {}", p);
  delete db;
}
