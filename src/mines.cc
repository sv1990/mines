#include "mines.hh"

#include <QHBoxLayout>
#include <QPushButton>

mines::mines() : QMainWindow(nullptr) {
  this->setWindowTitle("Mines");
  auto palette = this->palette();
  palette.setColor(QPalette::Background, QColor(55, 55, 55));
  this->setPalette(palette);

  int rows      = 16;
  int cols      = 30;
  int num_bombs = 99;

  _board      = new gameboard(rows, cols, num_bombs, this);
  _highscore  = new highscore;
  _timer      = new timer(_highscore->first(), _highscore->last(), this);
  _bomb_count = new bomb_count(num_bombs, this);

  auto top_bar    = new QWidget(this);
  auto top_layout = new QHBoxLayout(top_bar);
  top_bar->setLayout(top_layout);
  top_layout->addWidget(_timer);
  top_layout->addWidget(_bomb_count);

  _central_widget = new QWidget(this);
  _layout         = new QVBoxLayout(_central_widget);
  _layout->addWidget(top_bar);
  _layout->addWidget(_board);
  _central_widget->setLayout(_layout);

  auto bottom_bar    = new QWidget(this);
  auto bottom_layout = new QHBoxLayout(bottom_bar);
  bottom_layout->addStretch(1);
  auto highscore_button = new QPushButton(this);
  highscore_button->setText("&Highscores");
  bottom_layout->addWidget(highscore_button);
  auto restart_button = new QPushButton(this);
  restart_button->setText("&Restart");
  bottom_layout->addWidget(restart_button);
  _layout->addWidget(bottom_bar);

  this->setCentralWidget(_central_widget);
  this->setFixedSize(cols * 20, rows * 26);

  _open_all_shortcut = new QShortcut(QKeySequence(Qt::Key_O), this);
  connect(_open_all_shortcut, &QShortcut::activated, _board,
          &gameboard::open_around_all_numbers);

  connect(_board, &gameboard::game_started, _timer, &timer::start);
  connect(_board, &gameboard::game_done, _timer, &timer::stop);
  connect(_board, &gameboard::game_done, [&] {
    if (_board->lost()) {
      _bomb_count->set_text_color(Qt::red);
    }
    else {
      _bomb_count->set_text_color(Qt::green);
    }
  });
  connect(_board, &gameboard::game_done, this, &mines::add_highscore);
  connect(_board, &gameboard::marks_changed, _bomb_count,
          &bomb_count::count_changed);
  connect(highscore_button, &QPushButton::clicked, this,
          &mines::show_highscore);
  connect(restart_button, &QPushButton::clicked, _board, &gameboard::reset);
  connect(restart_button, &QPushButton::clicked, _timer, &timer::reset);
  connect(_board, &gameboard::resetted_bombs, _bomb_count,
          &bomb_count::restart);
}

void mines::show_highscore() {
  _highscore->show();
}

void mines::add_highscore() {
  if (_board->is_done() && !_board->lost() && !_board->cheated()) {
    _highscore->add(_timer->seconds());
    _highscore->show();
  }
}
