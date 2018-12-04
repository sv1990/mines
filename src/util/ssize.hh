#ifndef MINES_SRC_UTIL_SSIZE_HH_1543926219783410695_
#define MINES_SRC_UTIL_SSIZE_HH_1543926219783410695_

#include <iterator>
#include <limits>

#include <cassert>

namespace util {
template <typename T>
std::ptrdiff_t ssize(const T& x) noexcept {
  assert(x <= std::numeric_limits<std::ptrdiff_t>::max());
  using std::size;
  return static_cast<std::ptrdiff_t>(size(x));
}
} // namespace util

#endif // MINES_SRC_UTIL_SSIZE_HH_1543926219783410695_
