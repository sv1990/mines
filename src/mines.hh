#ifndef MINES_SRC_MINES_HH_1537684785294630186_
#define MINES_SRC_MINES_HH_1537684785294630186_

#include "bomb_count.hh"
#include "gameboard.hh"
#include "highscore.hh"
#include "timer.hh"

#include <QMainWindow>
#include <QVBoxLayout>
#include <QHBoxLayout>

class mines : public QMainWindow {
  QWidget* _central_widget;
  QVBoxLayout* _layout;

  gameboard* _board;
  timer* _timer;
  highscore* _highscore;
  bomb_count* _bomb_count;

public:
  mines() : QMainWindow(nullptr) {
    int rows        = 16;
    int cols        = 30;
    int num_bombs   = 99;
    _central_widget = new QWidget(this);
    this->setCentralWidget(_central_widget);
    this->setFixedSize(cols * 22, rows * 23);

    _layout     = new QVBoxLayout(this);
    _board      = new gameboard(rows, cols, num_bombs, this);
    _timer      = new timer(this);
    _highscore  = new highscore;
    _bomb_count = new bomb_count(num_bombs, this);

    auto top_bar = new QWidget(this);
    auto top_layout = new QHBoxLayout(this);
    top_bar->setLayout(top_layout);
    top_layout->addWidget(_timer);
    top_layout->addWidget(_bomb_count);

    _layout->addWidget(top_bar);
    _layout->addWidget(_board);

    _central_widget->setLayout(_layout);

    connect(_board, &gameboard::game_started, _timer, &timer::start);
    connect(_board, &gameboard::game_done, _timer, &timer::stop);
    connect(_board, &gameboard::game_done, this, &mines::show_highscore);
    connect(_board, &gameboard::marks_changed, _bomb_count, &bomb_count::count_changed);
  }

  void show_highscore() {
    if (!_board->lost()) {
      _highscore->add(_timer->seconds());
    }
    _highscore->show();
  }
};

#endif // MINES_SRC_MINES_HH_1537684785294630186_
