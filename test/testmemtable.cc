#include <assert.h>

#include <iostream>
#include <string>

#include "../db/db.h"
#include "../util/common.h"
#include "gtest/gtest.h"

TEST(testmemtable, test0) {
  mxcdb::DB *db;
  mxcdb::Options opt;
  mxcdb::State s = mxcdb::DBImpl::Open(opt, "/tmp/testdb", &db);
  assert(s.ok());

  // write key1,value1
  std::string key = "key";
  std::string value = "fdsfsd";
  std::string value1 = "fdsfsd";
  for (int i = 0; i < 10; i++) {
    key.append(std::to_string(i));
    value.append(std::to_string(i));
    s = db->Put(mxcdb::WriteOptions(), key, value);
    EXPECT_TRUE(s.ok());
  }
  assert(s.ok());
  std::string key1 = "key";
  for (int i = 0; i < 10; i++) {
    key1.append(std::to_string(i));
    s = db->Get(mxcdb::ReadOptions(), key1, &value);
    EXPECT_TRUE(s.ok());
    value1.append(std::to_string(i));
    EXPECT_EQ(value, value1);
  }
  db->Delete(mxcdb::WriteOptions(), "key0"); // iserror
  s = db->Get(mxcdb::ReadOptions(), "key0", &value);
  EXPECT_TRUE(s.IsNotFound());
  delete db;
}
