#ifndef MINES_SRC_ENTRY_HH_1537645458639141469_
#define MINES_SRC_ENTRY_HH_1537645458639141469_

#include <optional>
#include <variant>

class entry {
public:
  struct empty {};
  struct close_to {
    unsigned num;
  };
  struct bomb {};

  using value_t = std::variant<empty, close_to, bomb>;
  enum class state_t { hidden, marked, opened };

private:
  value_t _value;
  state_t _state;

public:
  entry& operator=(const value_t& rhs) noexcept {
    _value = rhs;
    return *this;
  }

  bool is_bomb() const noexcept { return std::holds_alternative<bomb>(_value); }
  bool is_empty() const noexcept {
    return std::holds_alternative<empty>(_value);
  }
  std::optional<unsigned> is_close_to() const noexcept {
    if (std::holds_alternative<close_to>(_value)) {
      return {std::get<close_to>(_value).num};
    }
    return {};
  }

  void open() noexcept { _state = state_t::opened; }

  /**
   * Mark entry
   *
   * @return 1 if marked, -1 if unmarked, 0 else
   */
  int mark() noexcept {
    switch (_state) {
    case state_t::hidden:
      _state = state_t::marked;
      return 1;
    case state_t::marked:
      _state = state_t::hidden;
      return -1;
    case state_t::opened:
      return 0;
    }
    __builtin_unreachable();
  }

  state_t state() const noexcept { return _state; }

  value_t value() const noexcept { return _value; }
};

#endif // MINES_SRC_ENTRY_HH_1537645458639141469_
