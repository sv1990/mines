#include "field.hh"

#include "log.hh"

#include "util/random_gen.hh"
#include "util/ssize.hh"

#include <range/v3/algorithm/count_if.hpp>
#include <range/v3/algorithm/fill.hpp>
#include <range/v3/algorithm/swap_ranges.hpp>
#include <range/v3/numeric/accumulate.hpp>
#include <range/v3/view/cartesian_product.hpp>
#include <range/v3/view/filter.hpp>
#include <range/v3/view/iota.hpp>
#include <range/v3/view/reverse.hpp>
#include <range/v3/view/transform.hpp>

#include <algorithm>
#include <array>
#include <functional>
#include <queue>

/**
 * Returns the a view of the indices of the surrounding fields of (row, col)
 */
auto adjacent_entries(const field& f, int row, int col) noexcept {
  assert(row >= 0 && row < f.rows());
  assert(col >= 0 && col < f.cols());

  return ranges::views::cartesian_product(ranges::views::ints(-1, 2),
                                          ranges::views::ints(-1, 2)) //
         | ranges::views::filter([](const auto& tpl) {
             const auto& [x, y] = tpl;
             return x != 0 || y != 0;
           }) //
         | ranges::views::transform([row, col](const auto& p) {
             auto [dr, dc] = p;
             return std::pair(row + dr, col + dc);
           }) //
         | ranges::views::filter([&f](const auto& point) {
             return f.at(point.first, point.second).has_value();
           });
}

void field::init(int row, int col) noexcept {
  assert(row >= 0 && row < _rows);
  assert(col >= 0 && col < _cols);
  auto clicked_fields = adjacent_entries(*this, row, col) | ranges::to_vector;
  clicked_fields.emplace_back(row, col);

  // Let n be the number of entries around (row, col) and itself. Place the
  // bombs in the beginning of the entries vector and shuffle them while keeping
  // the last n entries in its place. The last n entries are then swapped with
  // the clicked ones.

  std::fill_n(begin(_entries), _num_bombs, entry::bomb{});
  std::shuffle(begin(_entries),
               prev(end(_entries), util::ssize(clicked_fields)),
               util::random_gen());

  ranges::swap_ranges(
      clicked_fields //
          | ranges::views::transform([this](const auto& p) -> entry& {
              return (*this)(p.first, p.second);
            }),
      _entries | ranges::views::reverse);

  for (int irow = 0; irow < _rows; ++irow) {
    for (int icol = 0; icol < _cols; ++icol) {
      if ((*this)(irow, icol).is_bomb()) {
        continue;
      }
      auto count = count_bombs_adjacent_to(irow, icol);
      if (count > 0) {
        (*this)(irow, icol) = entry::close_to{count};
      }
    }
  }
}

void field::reset() noexcept {
  ranges::fill(_entries, entry{});
}

field::field(int rows, int cols, int num_bombs) noexcept
    : _entries(), _rows(rows), _cols(cols), _num_bombs(num_bombs) {
  assert(_rows > 0);
  assert(_cols > 0);
  assert(_num_bombs > 0);
  _entries.resize(static_cast<std::size_t>(_rows * _cols));
}

int field::count_bombs_adjacent_to(int row, int col) const noexcept {
  assert(row >= 0 && row < _rows);
  assert(col >= 0 && col < _cols);

  auto adjacent = adjacent_entries(*this, row, col);
  return static_cast<int>(
      ranges::count_if(adjacent, &entry::is_bomb, [this](auto&& p) {
        return this->at(p.first, p.second);
      }));
}

void field::open_empty_around(int row, int col) noexcept {
  assert((*this)(row, col).is_empty());
  // Perform breadth first search

  std::queue<std::pair<int, int>> todo;

  todo.push({row, col});
  (*this)(row, col).open();

  while (!empty(todo)) {
    auto next = todo.front();
    if ((*this)(next.first, next.second).is_empty()) {
      for (auto p : adjacent_entries(*this, next.first, next.second)) {
        if ((*this)(p).is_opened()) {
          continue;
        }
        (*this)(p.first, p.second).open();
        if ((*this)(p.first, p.second).is_empty()) {
          todo.push(p);
        }
      }
    }
    todo.pop();
  }
}

bool field::open(int row, int col) noexcept {
  assert(row >= 0 && row < _rows);
  assert(col >= 0 && col < _cols);

  auto& entry = (*this)(row, col);
  if (entry.is_hidden()) {
    entry.open();
  }
  else {
    return true;
  }
  if (entry.is_bomb()) {
    return false;
  }
  if (entry.is_empty()) {
    open_empty_around(row, col);
  }
  return true;
}

int field::mark(int row, int col) noexcept {
  assert(row >= 0 && row < _rows);
  assert(col >= 0 && col < _cols);
  return (*this)(row, col).mark();
}

bool field::open_around(int row, int col) noexcept {
  assert(row >= 0 && row < _rows);
  assert(col >= 0 && col < _cols);

  const auto& selected_entry = (*this)(row, col);

  bool alive = true;
  if (selected_entry.is_opened() && selected_entry.is_close_to()) {
    auto adjacent   = adjacent_entries(*this, row, col);
    auto mark_count = ranges::count_if(adjacent, &entry::is_marked,
                                       [this](auto&& p) { return (*this)(p); });
    if (mark_count == selected_entry.is_close_to().value()) {
      // Use accumulate instead of all_of to prevent short circuiting on the
      // first false return value
      alive = ranges::accumulate(
          adjacent //
              | ranges::views::filter(&entry::is_hidden,
                                      [this](auto&& p) { return (*this)(p); }),
          true, std::logical_and<bool>{},
          [this](const auto& p) { return this->open(p.first, p.second); });
    }
  }
  return alive;
}

bool field::is_done() const noexcept {
  const auto hidden = ranges::count_if(_entries, &entry::is_hidden);
  logger().info("number of bombs: {}; open fields: {}", _num_bombs, hidden);
  return hidden == _num_bombs;
}
