#ifndef MINES_SRC_UTIL_SSIZE_HH_1543926219783410695_
#define MINES_SRC_UTIL_SSIZE_HH_1543926219783410695_

#include <iterator>
#include <limits>
#include <type_traits>

#include <cassert>

namespace util {
using std::size;
template <typename T>
auto ssize(const T& x) noexcept
    -> std::common_type_t<std::ptrdiff_t,
                          std::make_signed_t<decltype(size(x))>> {
  using Ret =
      std::common_type_t<std::ptrdiff_t, std::make_signed_t<decltype(size(x))>>;
  assert(size(x) <=
         static_cast<decltype(size(x))>(std::numeric_limits<Ret>::max()));
  return static_cast<Ret>(size(x));
}
} // namespace util

#endif // MINES_SRC_UTIL_SSIZE_HH_1543926219783410695_
