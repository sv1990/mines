#include "field.hh"

#include "util/random_gen.hh"

#include <range/v3/algorithm/count_if.hpp>
#include <range/v3/algorithm/swap_ranges.hpp>
#include <range/v3/view/filter.hpp>
#include <range/v3/view/reverse.hpp>
#include <range/v3/view/take.hpp>
#include <range/v3/view/transform.hpp>

#include <algorithm>
#include <array>
#include <queue>

using namespace ranges;

void field::init(int row, int col) noexcept {
  assert(util::in_range(row, 0, _rows - 1));
  assert(util::in_range(col, 0, _cols - 1));
  auto clicked_fields = adjacent_entries(row, col);
  clicked_fields.emplace_back(row, col);

  std::fill_n(begin(_entries), _num_bombs, entry::bomb{});
  std::shuffle(begin(_entries),
               prev(end(_entries), static_cast<int>(size(clicked_fields))),
               util::random_gen());

  swap_ranges(clicked_fields | view::transform([this](const auto& p) -> entry& {
                return (*this)(p.first, p.second);
              }),
              _entries | view::reverse | view::take(size(clicked_fields)));

  for (int irow = 0; irow < _rows; ++irow) {
    for (int icol = 0; icol < _cols; ++icol) {
      if ((*this)(irow, icol).is_bomb()) {
        continue;
      }
      auto count = count_adjacent_bombs(irow, icol);
      if (count > 0) {
        (*this)(irow, icol) = entry::close_to{count};
      }
    }
  }
}

void field::reset() noexcept {
  _entries = std::vector<entry>(static_cast<std::size_t>(_rows * _cols));
}

field::field(int rows, int cols, int num_bombs) noexcept
    : _entries(), _rows(rows), _cols(cols), _num_bombs(num_bombs) {
  assert(_rows > 0);
  assert(_cols > 0);
  assert(_num_bombs > 0);
  reset();
}

std::vector<std::pair<int, int>> field::adjacent_entries(int row, int col) const
    noexcept {
  assert(util::in_range(row, 0, _rows - 1));
  assert(util::in_range(col, 0, _cols - 1));

  // clang-format off
  std::array<std::pair<int, int>, 8> indices{
      {{1, -1}, {1, 0}, {1, 1}, {0, -1}, {0, 1}, {-1, -1}, {-1, 0}, {-1, 1},}
  };
  // clang-format on
  return indices //
         | view::transform([row, col](const auto& p) {
             return std::pair<int, int>(row + p.first, col + p.second);
           }) //
         | view::filter([this](const auto& p) {
             return this->at(p.first, p.second).has_value();
           }) //
         | to_vector;
}

int field::count_adjacent_bombs(int row, int col) const noexcept {
  assert(util::in_range(row, 0, _rows - 1));
  assert(util::in_range(col, 0, _cols - 1));

  auto adjacent = adjacent_entries(row, col);
  return static_cast<int>(distance(adjacent //
                                   | view::transform([this](const auto& p) {
                                       const auto& [r, c] = p;
                                       return this->at(r, c);
                                     }) //
                                   | view::filter(&entry::is_bomb)));
}

bool field::open(int row, int col) noexcept {
  assert(util::in_range(row, 0, _rows - 1));
  assert(util::in_range(col, 0, _cols - 1));

  auto& entry = (*this)(row, col);
  if (entry.state() == entry::state_t::hidden) {
    entry.open();
  }
  else {
    return true;
  }
  if (entry.is_bomb()) {
    return false;
  }
  if (entry.is_empty()) {
    std::queue<std::pair<int, int>> todo;
    for (const auto& p : adjacent_entries(row, col)) {
      todo.push(p);
      (*this)(p.first, p.second).open();
    }
    while (!empty(todo)) {
      auto next = todo.front();
      if ((*this)(next.first, next.second).is_empty()) {
        for (const auto& p : adjacent_entries(next.first, next.second)) {
          if ((*this)(p.first, p.second).state() == entry::state_t::opened) {
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
  return true;
}

int field::mark(int row, int col) noexcept {
  assert(util::in_range(row, 0, _rows - 1));
  assert(util::in_range(col, 0, _cols - 1));
  return (*this)(row, col).mark();
}

bool field::open_around(int row, int col) noexcept {
  assert(util::in_range(row, 0, _rows - 1));
  assert(util::in_range(col, 0, _cols - 1));

  const auto& selected_entry = (*this)(row, col);

  bool alive = true;
  if (selected_entry.state() == entry::state_t::opened &&
      selected_entry.is_close_to()) {
    auto adjacent   = adjacent_entries(row, col);
    auto mark_count = ranges::count_if(adjacent, [this](const auto& p) {
      return (*this)(p.first, p.second).state() == entry::state_t::marked;
    });
    if (mark_count == selected_entry.is_close_to().value()) {
      for (const auto& [r, c] : adjacent_entries(row, col)) {
        if ((*this)(r, c).state() == entry::state_t::hidden) {
          // this->open(r, c) needs to be on the left hand side of && since its
          // side effects need to be performed
          alive = this->open(r, c) && alive;
        }
      }
    }
  }
  return alive;
}

bool field::is_done() const noexcept {
  return ranges::count_if(_entries, [](const auto& entry) {
           return entry.state() != entry::state_t::opened;
         }) == static_cast<long>(_num_bombs);
}
