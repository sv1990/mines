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

void gameboard::start(int row, int col) noexcept {
  if (!_started) {
    _started = true;
    _field.init(row, col);
    _cheated = false;
    emit game_started();
  }
}

void gameboard::open(int row, int col) noexcept {
  start(row, col);
  _lost = !_field.open(row, col);
  update_pixmaps();
  if (_lost || is_done()) {
    uncover();
    emit game_done();
  }
}

void gameboard::open_around(int row, int col) noexcept {
  start(row, col);
  _lost = !_field.open_around(row, col);
  update_pixmaps();
  if (_lost || is_done()) {
    uncover();
    emit game_done();
  }
}

void gameboard::open_around_all_numbers() noexcept {
  _cheated = true;
  for (int row = 0; row < _field.rows(); ++row) {
    for (int col = 0; col < _field.cols(); ++col) {
      if (_field(row, col).is_close_to()) {
        open_around(row, col);
      }
    }
  }
}

void gameboard::mark(int row, int col) noexcept {
  start(row, col);
  auto mark_change = _field.mark(row, col);
  if (mark_change != 0) {
    emit marks_changed(mark_change);
  }
  (*this)(row, col)->update_pixmap();
}

bool gameboard::is_bomb(int row, int col) const noexcept {
  return _field(row, col).is_bomb();
}

entry::state_t gameboard::state(int row, int col) noexcept {
  return _field(row, col).state();
}

entry::value_t gameboard::value(int row, int col) noexcept {
  return _field(row, col).value();
}

bool gameboard::is_done() const noexcept {
  return _lost || _field.is_done();
}
