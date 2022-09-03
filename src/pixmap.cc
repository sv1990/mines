#include "pixmap.hh"

#include "gameboard.hh"

#include "util/expect.hh"
#include "util/overloaded.hh"

#include <QPixmap>

#include <unordered_map>
#include <variant>

inline QPixmap make_pixmap(const char* filepath) noexcept {
  return QPixmap(filepath).scaled(
      16, 16, Qt::KeepAspectRatio, Qt::SmoothTransformation
  );
}

const QPixmap& get_pixmap(const std::string& name) noexcept {
  static const std::unordered_map<std::string, QPixmap> pixmaps{
      {           "1",            make_pixmap(":/images/1.png")},
      {           "2",            make_pixmap(":/images/2.png")},
      {           "3",            make_pixmap(":/images/3.png")},
      {           "4",            make_pixmap(":/images/4.png")},
      {           "5",            make_pixmap(":/images/5.png")},
      {           "6",            make_pixmap(":/images/6.png")},
      {           "7",            make_pixmap(":/images/7.png")},
      {           "8",            make_pixmap(":/images/8.png")},
      {        "bomb",         make_pixmap(":/images/bomb.png")},
      {       "empty",        make_pixmap(":/images/empty.png")},
      {      "hidden",       make_pixmap(":/images/hidden.png")},
      { "marked_bomb",  make_pixmap(":/images/marked_bomb.png")},
      {"marked_empty", make_pixmap(":/images/marked_empty.png")},
      {      "marked",       make_pixmap(":/images/marked.png")},
  };
  auto it = pixmaps.find(name);
  EXPECT(it != end(pixmaps));
  return it->second;
}

pixmap::pixmap(gameboard* field, int row, int col) noexcept
    : QLabel(field), _board(field), _row(row), _col(col) {
  this->setPixmap(get_pixmap("hidden"));
  this->setFixedHeight(16);
  this->setFixedWidth(16);
}

void pixmap::uncover() noexcept {
  auto is_bomb = _board->is_bomb(_row, _col);
  switch (_board->state(_row, _col)) {
  case entry::state_t::hidden:
    if (is_bomb) {
      this->setPixmap(get_pixmap("bomb"));
    }
    break;
  case entry::state_t::marked:
    if (is_bomb) {
      this->setPixmap(get_pixmap("marked_bomb"));
    }
    else {
      this->setPixmap(get_pixmap("marked_empty"));
    }
    break;
  case entry::state_t::opened:
    // This is needed since the last update_pixmaps is not called
    if (is_bomb) {
      this->setPixmap(get_pixmap("bomb"));
    }
    break;
  }
}

void pixmap::update_pixmap() {
  switch (_board->state(_row, _col)) {
  case entry::state_t::hidden:
    this->setPixmap(get_pixmap("hidden"));
    break;
  case entry::state_t::marked:
    this->setPixmap(get_pixmap("marked"));
    break;
  case entry::state_t::opened:
    std::visit(
        util::overloaded{
            [this](const entry::empty&) {
              this->setPixmap(get_pixmap("empty"));
            },
            [this](const entry::close_to& close_to) {
              this->setPixmap(get_pixmap(std::to_string(close_to.value)));
            },
            [this](const entry::bomb&) {
              this->setPixmap(get_pixmap("bomb"));
            }},
        _board->value(_row, _col)
    );
    break;
  }
}

template <typename... Ts>
bool is_clicked(Qt::MouseButtons clicked, Ts... buttons) {
  return ((clicked & buttons) && ...);
}

void pixmap::mousePressEvent(QMouseEvent* event) {
  if (!_board->is_active()) {
    return;
  }
  auto buttons = event->buttons();
  if (is_clicked(buttons, Qt::LeftButton, Qt::RightButton)
      || is_clicked(buttons, Qt::MiddleButton))
  {
    _board->open_around(_row, _col);
  }
  else if (is_clicked(buttons, Qt::LeftButton)) {
    _board->open(_row, _col);
  }
  else if (is_clicked(buttons, Qt::RightButton)) {
    _board->mark(_row, _col);
  }
}
