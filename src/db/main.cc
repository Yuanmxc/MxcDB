#include <assert.h>
#include <iostream>
#include <string.h>

#include "db.h"

int main() {
  mxcdb::DB *db;
  mxcdb::Options options;
  options.create_if_missing = true;
  mxcdb::State s = mxcdb::DB::Open(options, "/tmp/testdb", &db);
  assert(s.ok());

  // write key1,value1
  std::string key = "key";
  std::string value = "value";

  s = db->Put(mxcdb::WriteOptions(), key, value);
  assert(s.ok());
  return 0;
}