#ifndef MINES_SRC_ENTRY_HH_1537645458639141469_
#define MINES_SRC_ENTRY_HH_1537645458639141469_

#include <optional>
#include <variant>

class entry {
public:
  struct empty {};
  struct close_to {
    int num;
  };
  struct bomb {};

  using value_t = std::variant<empty, close_to, bomb>;
  enum class state_t { hidden, marked, opened };

private:
  value_t _value;
  state_t _state;

public:
  entry& operator=(const value_t& rhs) noexcept;

  bool is_bomb() const noexcept;
  bool is_empty() const noexcept;
  std::optional<unsigned> is_close_to() const noexcept;

  void open() noexcept;

  /**
   * Mark entry
   *
   * @return 1 if marked, -1 if unmarked, 0 else
   */
  int mark() noexcept;
  state_t state() const noexcept;
  value_t value() const noexcept;
};

#endif // MINES_SRC_ENTRY_HH_1537645458639141469_
