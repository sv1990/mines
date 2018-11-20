#ifndef MINES_SRC_UTIL_ENUMERATE_HH_1542724983641162179_
#define MINES_SRC_UTIL_ENUMERATE_HH_1542724983641162179_

#include <range/v3/view/indices.hpp>
#include <range/v3/view/zip.hpp>

namespace util {
template <typename Rng>
auto enumerate(const Rng& rng) {
  return ranges::view::zip(ranges::view::indices(ranges::size(rng)), rng);
}
} // namespace util

#endif // MINES_SRC_UTIL_ENUMERATE_HH_1542724983641162179_
