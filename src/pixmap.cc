#include "pixmap.hh"

#include "gameboard.hh"

#include "util/overloaded.hh"

#include <range/v3/algorithm/count_if.hpp>

pixmap::pixmap(gameboard* field, std::size_t row, std::size_t col) noexcept
    : QLabel(field), _board(field), _row(row), _col(col) {
  this->setPixmap(make_pixmap(":/images/hidden.png"));
  this->setFixedHeight(16);
  this->setFixedWidth(16);
}

void pixmap::update_pixmap() noexcept {
  switch (_board->state(_row, _col)) {
  case entry::state_t::hidden:
    this->setPixmap(make_pixmap(":/images/hidden.png"));
    break;
  case entry::state_t::marked:
    this->setPixmap(make_pixmap(":/images/marked.png"));
    break;
  case entry::state_t::opened:
    std::visit(
        util::overloaded{
            [this](const entry::empty&) {
              this->setPixmap(make_pixmap(":/images/empty.png"));
            },
            [this](const entry::close_to& close_to) {
              this->setPixmap(make_pixmap(
                  (":/images/" + std::to_string(close_to.num) + ".png")
                      .c_str()));
            },
            [this](const entry::bomb&) {
              this->setPixmap(make_pixmap(":/images/bomb.png"));
            }},
        _board->value(_row, _col));
    break;
  }
}

void pixmap::mousePressEvent(QMouseEvent* event) {
  auto buttons = event->buttons();
  if ((buttons & Qt::LeftButton && buttons & Qt::RightButton) ||
      buttons & Qt::MiddleButton) {
    _board->open_around(_row, _col);
  }
  else if (buttons & Qt::LeftButton) {
    _board->open(_row, _col);
  }
  else if (buttons & Qt::RightButton) {
    _board->mark(_row, _col);
  }
}
