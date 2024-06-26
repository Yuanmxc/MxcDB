#include "loger.h"
#include <iostream>
#include <spdlog/logger.h>

#include "spdlog/sinks/basic_file_sink.h"
namespace mxcdb {
Loger &Loger::Logger() {
  static Loger instance;
  return instance;
}
void Loger::Setlog(const std::string &db_name) {
  try {
    logger = spdlog::basic_logger_mt(db_name, "logs/worklog.log");
    logger->set_level(spdlog::level::debug);
    logger->set_pattern("[%Y-%m-%d %H:%M:%S.%e][thread %t][%@,%!][%l] : %v");
    logger->flush_on(spdlog::level::info);
    logger->flush_on(spdlog::level::debug);
    spdlog::flush_every(std::chrono::seconds(1));
  } catch (std::exception &ex) {
    std::cout << ex.what() << std::endl;
    exit(-1);
  }
}
} // namespace mxcdb