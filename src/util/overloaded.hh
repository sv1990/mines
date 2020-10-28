#ifndef MINES_SRC_OVERLOADED_HH_1537685620275731873_
#define MINES_SRC_OVERLOADED_HH_1537685620275731873_

namespace util {
template <class... Ts>
struct overloaded : Ts... {
  using Ts::operator()...;
};

template <class... Ts>
overloaded(Ts...) -> overloaded<Ts...>;
} // namespace util

#endif // MINES_SRC_OVERLOADED_HH_1537685620275731873_
