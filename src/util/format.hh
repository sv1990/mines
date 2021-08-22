#ifndef MINES_SRC_UTIL_FORMAT_HH_1576850149257570282_
#define MINES_SRC_UTIL_FORMAT_HH_1576850149257570282_

#include <fmt/format.h>

#include <QString>

#include <iterator>
#include <string_view>

namespace util {
template <typename... Args>
QString format(fmt::format_string<Args...> fmt, const Args&... args) {
  const auto sz = static_cast<int>(fmt::formatted_size(fmt, args...));
  QString result;
  result.reserve(sz);
  fmt::format_to(std::back_inserter(result), fmt, args...);
  return result;
}
} // namespace util

#endif // MINES_SRC_UTIL_FORMAT_HH_1576850149257570282_
