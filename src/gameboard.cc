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
  for (int irow = 0; irow < _layout->rowCount(); ++irow) {
    for (int icol = 0; icol < _layout->columnCount(); ++icol) {
      (*this)(irow, icol)->uncover();
    }
  }
}

void gameboard::mark_remaining_bombs() noexcept {
  for (int irow = 0; irow < _layout->rowCount(); ++irow) {
    for (int icol = 0; icol < _layout->columnCount(); ++icol) {
      if (is_bomb(irow, icol) && state(irow, icol) != entry::state_t::marked) {
        mark(irow, icol);
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
    emit game_started();
  }
}

void gameboard::check_if_game_is_done(int row, int col) {
  if (_lost || is_finished()) {
    if (_lost) {
      (*this)(row, col)->uncover_as_exploded();
      uncover();
    }
    else {
      mark_remaining_bombs();
    }
    emit game_done();
  }
}

void gameboard::open_field(int row, int col) noexcept {
  start(row, col);
  _lost = !_field.open(row, col);
  update_pixmaps();
  check_if_game_is_done(row, col);
}

void gameboard::open_around(int row, int col) noexcept {
  start(row, col);
  _lost = !_field.open_around(row, col);
  update_pixmaps();
  check_if_game_is_done(row, col);
}

void gameboard::open(int row, int col) noexcept {
  if (!_field(row, col).is_open()) {
    open_field(row, col);
  }
  else if (_field(row, col).is_close_to()) {
    open_around(row, col);
  }
}

void gameboard::mark(int row, int col) noexcept {
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

bool gameboard::is_finished() const noexcept {
  return _field.is_finished();
}

void gameboard::set_first_click_state(int state) {
  _field.set_first_click_state(static_cast<bool>(state));
}
