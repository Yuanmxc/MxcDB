#ifndef MXCDB_SNAPSHOT_H_
#define MXCDB_SHAPSHOT_H_
#include "../util/key.h"
namespace mxcdb {
class Snapshot {
public:
  explicit Snapshot(SequenceNum seq) : snap_seq(seq) {}
  ~Snapshot() = default;
  SequenceNum sequence() const { return snap_seq; }

private:
  friend class SnapshotList;
  const SequenceNum snap_seq;
};
} // namespace mxcdb
#endif
