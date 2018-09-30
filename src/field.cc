#include "field.hh"

#include "util/random_gen.hh"

#include <range/v3/all.hpp>

#include <algorithm>
#include <array>
#include <queue>

using namespace ranges;

void field::init(std::size_t row, std::size_t col) noexcept {
  if (_initialized) {
    return;
  }

  // TODO: It should be possible to optimize this using the functions below and
  // then removing the bombs from the clicked region
  // std::fill_n(begin(_entries), _num_bombs, entry::bomb{});
  // std::shuffle(begin(_entries), end(_entries), util::random_gen());

  auto clicked_fields = adjacent_entries(row, col);
  clicked_fields.emplace_back(row, col);

  auto bombs = _num_bombs;
  while (bombs > 0) {
    auto irow = std::uniform_int_distribution<std::size_t>{0, rows() - 1}(
        util::random_gen());
    auto icol = std::uniform_int_distribution<std::size_t>{0, cols() - 1}(
        util::random_gen());
    if (!(*this)(irow, icol).is_bomb() &&
        ranges::find(clicked_fields, std::pair{irow, icol}) ==
            ranges::end(clicked_fields)) {
      (*this)(irow, icol) = entry::bomb{};
      --bombs;
    }
  }

  for (std::size_t irow = 0; irow < _rows; ++irow) {
    for (std::size_t icol = 0; icol < _cols; ++icol) {
      if ((*this)(irow, icol).is_bomb()) {
        continue;
      }
      auto count = count_adjacent_bombs(irow, icol);
      if (count > 0) {
        (*this)(irow, icol) = entry::close_to{count};
      }
    }
  }
  _initialized = true;
}

field::field(std::size_t rows, std::size_t cols, std::size_t num_bombs) noexcept
    : _entries(rows * cols), _rows(rows), _cols(cols), _num_bombs(num_bombs) {
}

std::vector<std::pair<std::size_t, std::size_t>>
field::adjacent_entries(std::size_t row, std::size_t col) const noexcept {
  // clang-format off
  std::array<std::pair<long, long>, 8> indices{
      {{1, -1}, {1, 0}, {1, 1}, {0, -1}, {0, 1}, {-1, -1}, {-1, 0}, {-1, 1},}
  };
  // clang-format on
  return indices //
         | view::transform([row, col](const auto& p) {
             return std::pair<std::size_t, std::size_t>(row + p.first,
                                                        col + p.second);
           }) //
         | view::filter([this](const auto& p) {
             return this->at(p.first, p.second).has_value();
           }) //
         | to_vector;
}

unsigned field::count_adjacent_bombs(std::size_t row, std::size_t col) const
    noexcept {
  auto adjacent = adjacent_entries(row, col);
  return distance(adjacent //
                  | view::transform([this](const auto& p) {
                      const auto& [r, c] = p;
                      return this->at(r, c);
                    }) //
                  | view::filter(&entry::is_bomb));
}

bool field::open(std::size_t row, std::size_t col) noexcept {
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
    std::queue<std::pair<std::size_t, std::size_t>> todo;
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

int field::mark(std::size_t row, std::size_t col) noexcept {
  return (*this)(row, col).mark();
}

bool field::open_around(std::size_t row, std::size_t col) noexcept {
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
