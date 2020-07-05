#ifndef MINES_SRC_FIELD_HH_1537645753731530997_
#define MINES_SRC_FIELD_HH_1537645753731530997_

#include "entry.hh"

#include "util/expect.hh"

#include <vector>

#include <boost/optional.hpp>

/**
 * Implements game logic
 */
class field {
  std::vector<entry> _entries;
  int _rows;
  int _cols;
  int _num_bombs;
  bool _is_first_click_empty = true;

public:
  /**
   * Construct `width` x `height` minesweeper field with `num_bombs` bombs
   *
   */
  field(int rows, int cols, int num_bombs) noexcept;
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

private:
  template <typename Self>
  static auto& get_element_at(Self& self, int row, int col) noexcept {
    EXPECT(row >= 0 && row < self._rows);
    EXPECT(col >= 0 && col < self._cols);
    return self._entries[static_cast<std::size_t>(row * self._cols + col)];
  }

public:
  entry& operator()(int row, int col) noexcept {
    return get_element_at(*this, row, col);
  }
  const entry& operator()(int row, int col) const noexcept {
    return get_element_at(*this, row, col);
  }
  entry& operator()(const std::pair<int, int>& p) noexcept {
    return (*this)(p.first, p.second);
  }
  const entry& operator()(const std::pair<int, int>& p) const noexcept {
    return (*this)(p.first, p.second);
  }

  // std::optional doesn't support optional references
  boost::optional<const entry&> at(int row, int col) const noexcept {
    if (row >= 0 && row < _rows && col >= 0 && col < _cols) {
      return (*this)(row, col);
    }
    return {};
  }

  int count_bombs_adjacent_to(int row, int col) const noexcept;

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

  bool is_finished() const noexcept;

  void set_first_click_empty(bool is_empty) noexcept {
    _is_first_click_empty = is_empty;
  }
};

#endif // MINES_SRC_FIELD_HH_1537645753731530997_
