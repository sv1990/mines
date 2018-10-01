#ifndef MINES_SRC_FIELD_HH_1537645753731530997_
#define MINES_SRC_FIELD_HH_1537645753731530997_

#include "entry.hh"

#include "util/in_range.hh"

#include <optional>
#include <vector>

#include <cassert>

#include <iostream>

class field {
  std::vector<entry> _entries;
  int _rows;
  int _cols;
  int _num_bombs;

public:
  field(int width, int height, int num_bombs) noexcept;
  void init(int row, int col) noexcept;
  void reset() noexcept;

  int rows() const noexcept { return _rows; }
  int cols() const noexcept { return _cols; }
  int bombs() const noexcept { return _num_bombs; }

  entry& operator()(int row, int col) noexcept {
    assert(util::in_range(row, 0, _rows - 1));
    assert(util::in_range(col, 0, _cols - 1));
    return _entries[static_cast<std::size_t>(row * _cols + col)];
  }
  const entry& operator()(int row, int col) const noexcept {
    return const_cast<field&>(*this)(row, col);
  }

  std::optional<entry> at(int row, int col) const noexcept {
    if (row >= 0 && row < _rows && col >= 0 && col < _cols) {
      return (*this)(row, col);
    }
    return {};
  }

  std::vector<std::pair<int, int>> adjacent_entries(int row, int col) const
      noexcept;
  int count_adjacent_bombs(int row, int col) const noexcept;

  bool open(int row, int col) noexcept;
  bool open_around(int row, int col) noexcept;
  int mark(int row, int col) noexcept;

  bool is_done() const noexcept;
};

#endif // MINES_SRC_FIELD_HH_1537645753731530997_
