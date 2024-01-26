#ifndef MXCDB_SNAPSHOT_H_
#define MXCDB_SHAPSHOT_H_
namespace mxcdb {
class Snapshot {
  Snapshot() = default;
  virtual ~Snapshot();
};
class SnapshotImpl : public Snapshot {
  explicit SnapshotImpl() = default;
  ~SnapshotImpl();
};
} // namespace mxcdb
#endif