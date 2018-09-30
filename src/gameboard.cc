#include "gameboard.hh"

void gameboard::update_pixmaps() noexcept {
  // TODO: This is inefficient!
  for (int irow = 0; irow < _layout->rowCount(); ++irow) {
    for (int icol = 0; icol < _layout->columnCount(); ++icol) {
      (*this)(irow, icol)->update_pixmap();
    }
  }
}

void gameboard::uncover() noexcept {
  if (_lost) {
    for (int irow = 0; irow < _layout->rowCount(); ++irow) {
      for (int icol = 0; icol < _layout->columnCount(); ++icol) {
        (*this)(irow, icol)->uncover();
      }
    }
  }
}

void gameboard::reset() noexcept {
  _started = false;
  _lost    = false;
  _field.reset();
  update_pixmaps();
  emit resetted_bombs(_field.bombs());
}

void gameboard::start(std::size_t row, std::size_t col) noexcept {
  if (!_started) {
    _started = true;
    _field.init(row, col);
    emit game_started();
  }
}

void gameboard::open(std::size_t row, std::size_t col) noexcept {
  start(row, col);
  _lost = !_field.open(row, col);
  if (_lost || is_done()) {
    emit game_done();
    uncover();
  }
  else {
    update_pixmaps();
  }
}

void gameboard::open_around(std::size_t row, std::size_t col) noexcept {
  start(row, col);
  _lost = !_field.open_around(row, col);
  if (_lost || is_done()) {
    emit game_done();
    uncover();
  }
  else {
    update_pixmaps();
  }
}

void gameboard::mark(std::size_t row, std::size_t col) noexcept {
  start(row, col);
  auto mark_change = _field.mark(row, col);
  if (mark_change != 0) {
    emit marks_changed(mark_change);
  }
  (*this)(row, col)->update_pixmap();
}

bool gameboard::is_bomb(std::size_t row, std::size_t col) const noexcept {
  return _field(row, col).is_bomb();
}

entry::state_t gameboard::state(std::size_t row, std::size_t col) noexcept {
  return _field(row, col).state();
}

entry::value_t gameboard::value(std::size_t row, std::size_t col) noexcept {
  return _field(row, col).value();
}

bool gameboard::is_done() const noexcept {
  return _lost || _field.is_done();
}
