#ifndef MINES_SRC_FIELD_HH_1537645753731530997_
#define MINES_SRC_FIELD_HH_1537645753731530997_

#include "entry.hh"

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
  /**
   * Construct `width` x `height` minesweeper field with `num_bombs` bombs
   *
   */
  field(int width, int height, int num_bombs) noexcept;
  /**
   * Initializes the bombs on the field while keeping all fields surrounding
   * (row, col) empty
   *
   */
  void init(int row, int col) noexcept;
  void reset() noexcept;

  int rows() const noexcept { return _rows; }
  int cols() const noexcept { return _cols; }
  int bombs() const noexcept { return _num_bombs; }

  entry& operator()(int row, int col) noexcept {
    assert(row >= 0 && row < _rows);
    assert(col >= 0 && col < _cols);
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

  /**
   * Returns the indices of the surrounding fields of (row, col)
   */
  std::vector<std::pair<int, int>> adjacent_entries(int row, int col) const
      noexcept;
  int count_adjacent_bombs(int row, int col) const noexcept;

private:
  /**
   * Open all empty entries around (row, col)
   *
   */
  void open_empty_around(int row, int col) noexcept;

public:
  bool open(int row, int col) noexcept;
  /**
   * Opens all entries around (row, col) if it is open and empty
   *
   */
  bool open_around(int row, int col) noexcept;
  int mark(int row, int col) noexcept;

  bool is_done() const noexcept;
};

#endif // MINES_SRC_FIELD_HH_1537645753731530997_
