#ifndef MINES_SRC_RANDOM_GEN_HH_1537645952402413938_
#define MINES_SRC_RANDOM_GEN_HH_1537645952402413938_

#include <algorithm>
#include <functional>
#include <random>

namespace util {
/**
 * Generate a random seed sequence using a random_device to produce sufficient
 * entropy
 */
class random_seed_seq {
  std::random_device device;
  random_seed_seq() = default;

public:
  using result_type = typename std::random_device::result_type;

  template <typename It>
  void generate(It begin, It end) noexcept {
    std::generate(begin, end, std::ref(device));
  }

  static random_seed_seq& get_instance() noexcept {
    static thread_local random_seed_seq result;
    return result;
  }
};

inline auto& random_gen() noexcept {
  static thread_local std::mt19937 gen{random_seed_seq::get_instance()};
  return gen;
}
} // namespace util

#endif // MINES_SRC_RANDOM_GEN_HH_1537645952402413938_
