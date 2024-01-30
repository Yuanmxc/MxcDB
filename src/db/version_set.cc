#include "version_set.h"
namespace mxcdb {
int VersionSet::NumLevelFiles(int level) const {
  return nowversion->files[level].size();
}
} // namespace mxcdb