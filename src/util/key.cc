#include "key.h"
namespace mxcdb {
uint64_t InternalKey::parser(SequenceNum num, Valuetype type) {
  assert(num <= kMaxSequenceNumber);
  assert(type <= kTypeValue);
  return (num << 8) | type;
}
} // namespace mxcdb