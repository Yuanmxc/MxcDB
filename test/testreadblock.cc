#include "src/db/db.h"
#include "src/util/common.h"
#include "src/util/loger.h"
#include "gtest/gtest.h"
using namespace mxcdb;
TEST(testReadBlock, base) {
  mxcdb::DB *db;
  mxcdb::Options opt;
  mxcdb::State s = mxcdb::DBImpl::Open(opt, "/tmp/testdb", &db);
  assert(s.ok());

  // write key1,value1
  std::string key = "key0";
  std::string value = "value0";
  size_t p = 0;
  for (int i = 0; i < 200; i++) {
    s = db->Put(mxcdb::WriteOptions(), key, value);
    key.replace(3, std::to_string(i).size(), std::to_string(i));
    value.replace(5, std::to_string(i).size(), std::to_string(i));
    EXPECT_TRUE(s.ok());
  }
  key = "key0";
  value = "value0";
  sleep(1); // wait flush sstable
  // fix
  for (int i = 0; i < 200; i + 10) {
    s = db->Get(mxcdb::ReadOptions(), key1, &value);
    EXPECT_TRUE(s.ok());
    value1.append(std::to_string(i));
    EXPECT_EQ(value, value1);
  }
  delete db;
}
TEST(testReadTable, base) {}