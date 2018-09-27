#ifndef MINES_SRC_FIELD_HH_1537645753731530997_
#define MINES_SRC_FIELD_HH_1537645753731530997_

#include "entry.hh"

#include <optional>
#include <vector>

class field {
  std::vector<entry> _entries;
  std::size_t _rows;
  std::size_t _cols;
  std::size_t _num_bombs;

public:
  field(std::size_t width, std::size_t height, std::size_t num_bombs) noexcept;

  std::size_t rows() const noexcept { return _rows; }
  std::size_t cols() const noexcept { return _cols; }

  entry& operator()(std::size_t row, std::size_t col) noexcept {
    return _entries[row * _cols + col];
  }
  const entry& operator()(std::size_t row, std::size_t col) const noexcept {
    return const_cast<field&>(*this)(row, col);
  }

  std::optional<entry> at(std::size_t row, std::size_t col) const noexcept {
    if (row < _rows && col < _cols) {
      return (*this)(row, col);
    }
    return {};
  }

  std::vector<std::pair<std::size_t, std::size_t>>
  adjacent_entries(std::size_t row, std::size_t col) const noexcept;
  unsigned count_adjacent_bombs(std::size_t row, std::size_t col) const
      noexcept;

  bool open(std::size_t row, std::size_t col) noexcept;
  void open_around(std::size_t row, std::size_t col) noexcept;
  void mark(std::size_t row, std::size_t col) noexcept;

};

#endif // MINES_SRC_FIELD_HH_1537645753731530997_
