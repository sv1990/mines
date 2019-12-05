#include "log.hh"

#include <spdlog/sinks/basic_file_sink.h>

#include <QStandardPaths>

spdlog::logger& logger() {
  static auto logger = spdlog::basic_logger_mt(
      "log", QStandardPaths::writableLocation(QStandardPaths::CacheLocation)
                     .toStdString() +
                 "/log");
  return *logger;
}
