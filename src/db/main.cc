#include <assert.h>
#include <iostream>
#include <string>

#include "../util/common.h"
#include "db.h"
#include "writebatch.h"
int main() {
  mxcdb::DB *db;
  mxcdb::Options opt;
  mxcdb::State s = mxcdb::DBImpl::Open(opt, "/tmp/testdb", &db);
  assert(s.ok());

  // write key1,value1
  std::string key = "key";
  std::string value = "value";

  s = db->Put(mxcdb::WriteOptions(), key, value);
  assert(s.ok());
  return 0;
}