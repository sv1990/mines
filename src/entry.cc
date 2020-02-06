#include "entry.hh"

entry& entry::operator=(const value_t& rhs) noexcept {
  _value = rhs;
  return *this;
}

bool entry::is_bomb() const noexcept {
  return std::holds_alternative<bomb>(_value);
}
bool entry::is_empty() const noexcept {
  return std::holds_alternative<empty>(_value);
}
std::optional<unsigned> entry::is_close_to() const noexcept {
  if (std::holds_alternative<close_to>(_value)) {
    return {std::get<close_to>(_value).value};
  }
  return {};
}

bool entry::is_hidden() const noexcept {
  return _state == state_t::hidden;
}
bool entry::is_marked() const noexcept {
  return _state == state_t::marked;
}
bool entry::is_open() const noexcept {
  return _state == state_t::opened;
}

void entry::open() noexcept {
  _state = state_t::opened;
}

int entry::mark() noexcept {
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

entry::state_t entry::state() const noexcept {
  return _state;
}

entry::value_t entry::value() const noexcept {
  return _value;
}
