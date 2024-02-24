#ifndef MXCDB_LOGER_H
#define MXCDB_LOGER_H

#define SPDLOG_ACTIVE_LEVEL SPDLOG_LEVEL_TRACE
#include <spdlog/spdlog.h>

#include <string>
#include <string_view>

namespace mxcdb {
class Loger {
public:
  static Loger &Logger();
  Loger() = default;
  void Setlog(const std::string &dbname);
  std::shared_ptr<spdlog::logger> logger;
};
#define mlogger Loger::Logger()
#define mlog Loger::Logger().logger
} // namespace mxcdb
#endif