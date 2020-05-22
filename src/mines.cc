#include "mines.hh"

#include "util/make_qobject.hh"

#include <QCheckBox>
#include <QHBoxLayout>
#include <QPushButton>

mines::mines() : QMainWindow(nullptr) {
  this->setWindowTitle("Mines");
  auto palette = this->palette();
  palette.setColor(QPalette::Window, QColor(55, 55, 55));
  this->setPalette(palette);

  int rows      = 16;
  int cols      = 30;
  int num_bombs = 99;

  _board     = util::make_qobject<gameboard>(rows, cols, num_bombs, this);
  _highscore = util::make_qobject<highscore>();
  _timer =
      util::make_qobject<timer>(_highscore->first(), _highscore->last(), this);
  _bomb_count = util::make_qobject<bomb_count>(num_bombs, this);

  auto top_bar    = util::make_qobject<QWidget>(this);
  auto top_layout = util::make_qobject<QHBoxLayout>(top_bar);
  top_bar->setLayout(top_layout);
  top_layout->addWidget(_timer);
  top_layout->addWidget(_bomb_count);

  _central_widget = util::make_qobject<QWidget>(this);
  _layout         = util::make_qobject<QVBoxLayout>(_central_widget);
  _layout->addWidget(top_bar);
  _layout->addWidget(_board);
  _central_widget->setLayout(_layout);

  auto bottom_bar    = util::make_qobject<QWidget>(this);
  auto bottom_layout = util::make_qobject<QHBoxLayout>(bottom_bar);
  bottom_layout->addStretch(1);
  auto init_check_box =
      util::make_qobject<QCheckBox>("&First click is never a bomb", this);
  init_check_box->setChecked(true);
  bottom_layout->addWidget(init_check_box);
  auto highscore_button = util::make_qobject<QPushButton>(this);
  highscore_button->setText("&Highscores");
  bottom_layout->addWidget(highscore_button);
  auto restart_button = util::make_qobject<QPushButton>(this);
  restart_button->setText("&Restart");
  bottom_layout->addWidget(restart_button);
  _layout->addWidget(bottom_bar);

  this->setCentralWidget(_central_widget);
  this->setFixedSize(cols * 20, rows * 27);

  connect(_board, &gameboard::game_started, _timer, &timer::start);
  connect(_board, &gameboard::game_done, _timer, &timer::stop);
  connect(_board, &gameboard::game_done, [&] {
    if (_board->is_lost()) {
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
  connect(init_check_box, &QCheckBox::stateChanged, _board,
          &gameboard::set_first_click_state);
}

void mines::show_highscore() {
  _highscore->show();
}

void mines::add_highscore() {
  if (_board->is_finished() && !_board->is_lost()) {
    _highscore->add(_timer->seconds());
    _highscore->show();
  }
}
