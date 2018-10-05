#include "mines.hh"

#include <QHBoxLayout>
#include <QPushButton>
#include <QSpinBox>

mines::mines() noexcept : QMainWindow(nullptr) {
  int rows      = 16;
  int cols      = 30;
  int num_bombs = 99;

  _board      = new gameboard(rows, cols, num_bombs, this);
  _timer      = new timer(this);
  _highscore  = new highscore(rows, cols, num_bombs);
  _bomb_count = new bomb_count(num_bombs, this);

  auto top_bar    = new QWidget(this);
  auto top_layout = new QHBoxLayout(top_bar);
  top_bar->setLayout(top_layout);

  auto rows_box = new QSpinBox(top_bar);
  rows_box->setValue(rows);
  top_layout->addWidget(rows_box);
  auto cols_box = new QSpinBox(top_bar);
  cols_box->setValue(cols);
  top_layout->addWidget(cols_box);
  auto bombs_box = new QSpinBox(top_bar);
  bombs_box->setValue(num_bombs);
  top_layout->addWidget(bombs_box);

  top_layout->addWidget(_timer);
  top_layout->addWidget(_bomb_count);

  _central_widget = new QWidget(this);
  _layout         = new QVBoxLayout(_central_widget);
  _layout->addWidget(top_bar);
  _layout->addWidget(_board);
  _central_widget->setLayout(_layout);

  auto bottom_bar    = new QWidget(this);
  auto bottom_layout = new QHBoxLayout(bottom_bar);
  // bottom_layout->addSpacing(256);
  auto highscore_button = new QPushButton(this);
  highscore_button->setText("Show Highscore");
  bottom_layout->addWidget(highscore_button);
  auto restart_button = new QPushButton(this);
  restart_button->setText("Restart");
  bottom_layout->addWidget(restart_button);
  _layout->addWidget(bottom_bar);

  this->setCentralWidget(_central_widget);
  // this->setFixedSize(cols * 20, rows * 26);

  connect(_board, &gameboard::game_started, _timer, &timer::start);
  connect(_board, &gameboard::game_done, _timer, &timer::stop);
  connect(_board, &gameboard::game_done, this, &mines::add_highscore);
  connect(_board, &gameboard::marks_changed, _bomb_count,
          &bomb_count::count_changed);
  connect(highscore_button, &QPushButton::clicked, this,
          &mines::show_highscore);
  connect(restart_button, &QPushButton::clicked, _board, &gameboard::reset);
  connect(restart_button, &QPushButton::clicked, _timer, &timer::reset);
  connect(_board, &gameboard::resetted_bombs, _bomb_count,
          &bomb_count::restart);
  connect(rows_box, QOverload<int>::of(&QSpinBox::valueChanged), this,
          &mines::set_rows);
  connect(cols_box, QOverload<int>::of(&QSpinBox::valueChanged), this,
          &mines::set_cols);
  connect(bombs_box, QOverload<int>::of(&QSpinBox::valueChanged), this,
          &mines::set_bombs);
}

void mines::show_highscore() noexcept {
  _highscore->show();
}

void mines::add_highscore() noexcept {
  if (_board->is_done() && !_board->lost()) {
    _highscore->add(_timer->seconds());
    _highscore->show();
  }
}

void mines::set_rows(int rows) {
  _board->set_rows(rows);
}
void mines::set_cols(int cols) {
  _board->set_cols(cols);
}
void mines::set_bombs(int bombs) {
  _board->set_bombs(bombs);
}
