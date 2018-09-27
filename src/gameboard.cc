#include "gameboard.hh"

void gameboard::update_pixmaps() noexcept {
  // TODO: This is inefficient!
  for (int irow = 0; irow < _layout->rowCount(); ++irow) {
    for (int icol = 0; icol < _layout->columnCount(); ++icol) {
      (*this)(irow, icol)->update_pixmap();
    }
  }
}

void gameboard::open(std::size_t row, std::size_t col) noexcept {
  _field.open(row, col);
  update_pixmaps();
}

void gameboard::open_around(std::size_t row, std::size_t col) noexcept {
  _field.open_around(row, col);
  update_pixmaps();
}

void gameboard::mark(std::size_t row, std::size_t col) noexcept {
  _field.mark(row, col);
  (*this)(row, col)->update_pixmap();
}

entry::state_t gameboard::state(std::size_t row, std::size_t col) noexcept {
  return _field(row, col).state();
}

entry::value_t gameboard::value(std::size_t row, std::size_t col) noexcept {
  return _field(row, col).value();
}
