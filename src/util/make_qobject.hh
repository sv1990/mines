#ifndef MINES_SRC_UTIL_MAKE_QOBJECT_HH_1579011197024269193_
#define MINES_SRC_UTIL_MAKE_QOBJECT_HH_1579011197024269193_

#include <utility>

namespace util {
template <typename T, typename... Ts>
[[nodiscard]] T* make_qobject(Ts&&... xs) {
  return new T(std::forward<Ts>(xs)...);
}
} // namespace util

#endif // MINES_SRC_UTIL_MAKE_QOBJECT_HH_1579011197024269193_
