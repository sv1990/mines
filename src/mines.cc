#include "mines.hh"

#include <QHBoxLayout>
#include <QPushButton>

mines::mines() noexcept : QMainWindow(nullptr) {
  int rows      = 16;
  int cols      = 30;
  int num_bombs = 99;

  _board      = new gameboard(rows, cols, num_bombs, this);
  _timer      = new timer(this);
  _highscore  = new highscore;
  _bomb_count = new bomb_count(num_bombs, this);

  auto top_bar        = new QWidget(this);
  auto top_layout     = new QHBoxLayout(top_bar);
  auto restart_button = new QPushButton(this);
  restart_button->setText("Restart");
  top_bar->setLayout(top_layout);
  top_layout->addWidget(_timer);
  top_layout->addWidget(restart_button);
  top_layout->addWidget(_bomb_count);

  _central_widget = new QWidget(this);
  _layout         = new QVBoxLayout(_central_widget);
  _layout->addWidget(top_bar);
  _layout->addWidget(_board);
  _central_widget->setLayout(_layout);

  this->setCentralWidget(_central_widget);
  this->setFixedSize(cols * 22, rows * 23);

  connect(_board, &gameboard::game_started, _timer, &timer::start);
  connect(_board, &gameboard::game_done, _timer, &timer::stop);
  connect(_board, &gameboard::game_done, this, &mines::show_highscore);
  connect(_board, &gameboard::marks_changed, _bomb_count,
          &bomb_count::count_changed);
  connect(restart_button, &QPushButton::clicked, _board, &gameboard::reset);
  connect(restart_button, &QPushButton::clicked, _timer, &timer::reset);
  connect(_board, &gameboard::resetted_bombs, _bomb_count, &bomb_count::restart);
}

void mines::show_highscore() noexcept {
  if (!_board->lost()) {
    _highscore->add(_timer->seconds());
  }
  _highscore->show();
}
