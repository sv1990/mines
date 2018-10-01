#ifndef MINES_SRC_UTIL_IN_RANGE_HH_1538393095021129224_
#define MINES_SRC_UTIL_IN_RANGE_HH_1538393095021129224_

namespace util {
template <typename T>
bool in_range(const T& x, const T& min, const T& max) noexcept {
  return x >= min && x <= max;
}
} // namespace util

#endif // MINES_SRC_UTIL_IN_RANGE_HH_1538393095021129224_
